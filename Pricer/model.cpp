#include "model.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "pricer_exception.h"
#include "black_scholes.h"
#include "normal.h"
#include "brownian.h"
#include <algorithm>
#include <format>

std::string
enumToText(model_type mt)
{
	switch (mt)
	{
	case model_type::INVALID:
		return "INVALID";
	case model_type::BLACK_SCHOLES:
		return "BS";
	case model_type::NORMAL:
		return "NORMAL";
	default:
		THROW("Unkown model type?");
	}

}

model::model(const product& product, const numerical_parameters& np) :
	product_(product), numerical_parameters_(np), asset_(product.get_fx().get_asset()), basis_(product.get_fx().get_basis()),
	eps_percent_dt_(0.01), eps_percent_dx_(0.01)
{
	//do not call intialize as the real model has not finished initializing
}

static bool 
is_near_critical_point(double x, const std::set<double>& critical_points, double eps)
{
	for (const auto cx : critical_points)
		if (std::fabs(x - cx) < eps)
			return true;
	return false;
}

static
void initialize_points(std::vector<double>& v, double min, double max, int nb_points, const std::set<double>& critical_points, double percent_dx)
{
	if (nb_points < 2)
		THROW("Not enough points");
	double dx = (max - min) / (nb_points - 1);
	std::set<double> s;
	for (int i = 0; i < nb_points; ++i)
	{
		double x = min + i * dx;
		if (!is_near_critical_point(x, critical_points, dx*percent_dx))
			s.insert(x);
	}
	for (const auto cx : critical_points)
		s.insert(cx);
	v.resize(s.size());
	std::copy(s.begin(), s.end(), v.begin());
}

void
model::initialize_common()
{
	initialize_points(t_, 0.0, product_.get_expiry(), numerical_parameters_.time_points_, product_.get_critical_times(), eps_percent_dt_);

	// On a: S(t) = S0.Dfa(t)/Dfb(t)
	// On a: S(t+dt)/S(t) = [Dfa(t+dt)/Dfb(t+dt)] / [Dfa(t)/Dfb(t)] = q(t) 
	// On a: S(t+dt)  = S(t).q(t)
	// dS = S(t+dt)-S(t) = S(t).(q(t)-1) = S(t).r(t).dt avec r(t).dt = q(t)-1 donc r(t)=(q(t)-1)/dt
	//
	// Si Df(t)=exp(-r.t)
	// alors S(t) = S0.exp((-ra+rb).t)
	// alors S(t+dt) = S(t).exp((-ra+rb).dt) 
	// ca donne r(t) = -ra + rb


	r_.resize(t_.size());
	r_log_.resize(t_.size());
	r_basis_.resize(t_.size());
	r_asset_.resize(t_.size());
	double prev_ratio = 1.0;
	r_[0] = 0.0;
	for (int i = 1; i < r_.size(); ++i)
	{
		double ratio = asset_.get_df(t_[i]) / basis_.get_df(t_[i]);
		r_[i] = (ratio / prev_ratio - 1.0) / (t_[i] - t_[i - 1]);
		r_log_[i] = std::log(ratio / prev_ratio) / (t_[i] - t_[i - 1]);
		prev_ratio = ratio;
		r_basis_[i] = (basis_.get_df(t_[i]) / basis_.get_df(t_[i - 1]) - 1.0) / (t_[i] - t_[i - 1]);
		r_asset_[i] = (asset_.get_df(t_[i]) / asset_.get_df(t_[i - 1]) - 1.0) / (t_[i] - t_[i - 1]);
	}
}

void 
model::initialize_edp_coeffs()
{
	// A l'ordre 1 ou 2, on doit approximer la derivee et derive seconde
	// on connait 3 points a<b<c et f(a), f(b), f(c)
	// on doit approximer f'(b) et f''(b)
	// f'(b) ~ (f(c)-f(a))/(c-a) = f(a)/(a-c) + 0*f(b) + f(c)/(c-a) 
	// f''(b) ~ [(f(c) - f(b)) / (c - b) - (f(b) - f(a)) / (b - a)] / [(c - a) / 2] on considere la derivee en (c+b)/2 et (a+b)/2
	// f''(b) ~ f(a). 2 / [(a - b) * (a - c)] + f(b). 2 / [(b - a) * (b - c)] + f(c). 2 / [(c - a) * (c - b)]

	edp_coeffs_.resize(x_.size());
	for (int i = 1; i < x_.size() - 1; ++i)
	{
		double a = x_[i - 1];
		double b = x_[i];
		double c = x_[i + 1];

		edp_coeffs_[i].c1a_ = 1.0 / (a - c);
		edp_coeffs_[i].c1b_ = 0.0;
		edp_coeffs_[i].c1c_ = 1.0 / (c - a);

		edp_coeffs_[i].c2a_ = 2.0 / ((a - b) * (a - c));
		edp_coeffs_[i].c2b_ = 2.0 / ((b - a) * (b - c));
		edp_coeffs_[i].c2c_ = 2.0 / ((c - a) * (c - b));
	}
}

static double 
get_dv_avg(const std::vector<double>& v)
{
	return (*v.rbegin() - *v.begin()) / (v.size() - 1);
}

void
model::check_edp_params() const
{
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	if (params.schema_ == schema_type::EXPLICIT)
	{
		// We must have dt <= 0.5*dx^2
		double dt_avg = get_dv_avg(t_);
		double dx_avg = get_dv_avg(x_);
		if (dt_avg >= 0.5 * dx_avg * dx_avg)
		{
			THROW(std::format("dt/dx invalides, dt={}, dx^2={}", dt_avg, dx_avg * dx_avg));
		}
	}
}

void
model::initialize_edp()
{
	initialize_common();
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	double x_min, x_max;
	get_edp_xbounds(x_min, x_max);
	initialize_points(x_, x_min, x_max, params.x_points_, product_.get_critical_strikes(pde_underlying::FXSPOT), eps_percent_dx_);
	initialize_edp_coeffs();
	initialize_terminal_payoff_and_bounds();
	check_edp_params();
}

double
model::evaluate_edp() const
{
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	std::vector<double> V = V_terminal_;	
	for (size_t j = t_.size() - 1; j >= 1; --j)
	{
		double dt = t_[j] - t_[j-1];
		double r = r_[j]; // rem r[0] est indefini
		// on back-propagate : V(j) => V(j-1)
		switch (params.schema_)
		{
		case schema_type::EXPLICIT:
			back_propagate_explicit(V, dt, r);
			break;
		case schema_type::IMPLICIT:
			back_propagate_implicit(V, dt, r);
			break;
		case schema_type::CRANK_NICHOLSON:
			back_propagate_cranck_nicholson(V, dt, r);
			break;
		default:
			THROW("Unkwon schema");
		}
	}
	// Interpoler , rajouter spot dans grid ?
	// On renvoie V[t=0, x=Spot]
	auto it = std::lower_bound(x_.begin(), x_.end(), product_.get_fx().get_spot()); // value >= spot
	if (it == x_.end())
		THROW("Cannot find spot value");
	int index = (int)(it - x_.begin());
	if (index > 0)
	{
		double lambda = (x_[index] - product_.get_fx().get_spot()) / (x_[index] - x_[index - 1]);
		double value = V[index] + lambda * (V[index - 1] - V[index]);
		// l'EDP "discounte" a fxspot rate donc fwd => spot, donc on remultiplie par fwd * df_base = df_asset
		return value * asset_.get_df(product_.get_expiry()); 
	}
	else
	{
		THROW("Bad bounds?");
	}
}

void
model::initialize_terminal_payoff_and_bounds()
{
	V_terminal_.resize(x_.size());
	for (int i = 0; i < x_.size(); ++i)
		V_terminal_[i] = product_.payoff(x_[i]);

	V_bound_up_.resize(t_.size());
	V_bound_down_.resize(t_.size());

	// On suppose que aux bornes up et down, la vol n'a plus d'importance sur le payoff
    // reste a determiner dV/dS aux bornes
    // V(t) = V(t + dt) - dt.(r.V - r.S.dV / dS)
	// V(t) = V(t + dt) - r.dt.(V - S . dV/dS)
	V_bound_up_[t_.size() - 1] = V_terminal_[x_.size() - 1];
	V_bound_down_[t_.size() - 1] = V_terminal_[0];
	double S_up = x_[x_.size() - 1];
	double dvds_up = (V_terminal_[x_.size() - 1] - V_terminal_[x_.size() - 2]) / (x_[x_.size() - 1] - x_[x_.size() - 2]);
	double S_down = x_[0];
	double dvds_down = (V_terminal_[1] - V_terminal_[0]) / (x_[1] - x_[0]);
	for (size_t j = t_.size() - 1; j >= 1; --j)
	{
		double dt = t_[j] - t_[j - 1];
		double r = r_[j]; // rem r[0] est indefini
		V_bound_up_[j - 1] = V_bound_up_[j] - r * dt * (V_bound_up_[j] - S_up * dvds_up);
		V_bound_down_[j - 1] = V_bound_down_[j] - r * dt * (V_bound_down_[j] - S_down * dvds_down);
	}
}

void
model::initialize_mc()
{
	initialize_common();
}

void
model::initialize()
{
	switch (get_numerical_method())
	{
	case numerical_method::CLOSED_F:
		return;
	case numerical_method::EDP:
		return initialize_edp();
	case numerical_method::MC:
		return initialize_mc();
	default:
		THROW("Unkown numerical method?");
	}
}

double
model::evaluate() const
{
	switch (get_numerical_method())
	{
	case numerical_method::CLOSED_F:
		return evaluate_closed_f();
	case numerical_method::EDP:
		return evaluate_edp();
	case numerical_method::MC:
		return evaluate_mc();
	default:
		THROW("Unkown numerical method?");
	}
}

double
model::evaluate_mc() const
{
	const numerical_parameters_mc& params = dynamic_cast<const numerical_parameters_mc&>(numerical_parameters_);
	double sum_payoffs = 0.0;
	brownian b;
	for (int simul = 0; simul < params.simuls_; ++simul)
	{
		double St = product_.get_fx().get_spot();
		for (size_t j = 1; j < t_.size(); ++j)
		{
			// on diffuse le spot entre t[j-1] et t[j]
			double dt = t_[j] - t_[j - 1];
			double dS_deter = St * r_[j] * dt;
			double dS_stochastic = get_dS_mc(b.normal_value(), St, dt);
			St += dS_deter + dS_stochastic;
		}
		sum_payoffs += product_.payoff(St);
	}
	return (sum_payoffs / params.simuls_) * basis_.get_df(product_.get_expiry());
}

