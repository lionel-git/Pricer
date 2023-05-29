// The edp specific methods for class model
#include "model.h"
#include "numerical_parameters_edp.h"

#include <format>

void
model::initialize_edp()
{
	initialize_common();
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	double x_min, x_max;
	get_edp_xbounds(x_min, x_max);
	initialize_grid_axis(x_, x_min, x_max, params.x_points_, product_.get_critical_strikes(pde_underlying::FXSPOT), eps_percent_dx_);
	initialize_edp_coeffs();
	initialize_terminal_payoff_and_bounds();
	check_edp_params();
}

void 
model::set_theta(double theta)
{
	if (theta<0.0 || theta>1.0)
		THROW("Invalid theta value");
	theta_ = theta;
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
	for (size_t i = 1; i < x_.size() - 1; ++i)
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

double
model::evaluate_edp() const
{
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	std::vector<double> V = V_terminal_;
	for (size_t j = t_.size() - 1; j >= 1; --j)
	{
		// j   => t+dt
		// j-1 => t
		double dt = t_[j] - t_[j - 1];        // dt>0
		double r = r_[j];                     // rem: r[0] est indefini
		double U_up = V_bound_up_[j - 1];     // at t
		double U_down = V_bound_down_[j - 1]; // at t
		// on back-propagate : V(t+dt) (j) => V(t) (j-1)
		switch (params.schema_)
		{
		case schema_type::EXPLICIT:
			back_propagate_explicit(V, dt, r, U_up, U_down);
			break;
		case schema_type::IMPLICIT:
			back_propagate_implicit(V, dt, r, U_up, U_down);
			break;
		case schema_type::CRANK_NICHOLSON:
			back_propagate_cranck_nicholson(V, dt, r, U_up, U_down);
			break;
		default:
			THROW("Unkwon schema");
		}
	}
	// Interpoler , rajouter spot dans grid ?
	// On renvoie V[t=0, x=Spot]
	auto it = std::lower_bound(x_.begin(), x_.end(), product_.get_fx().get_spot()); // get first value >= spot
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
	for (size_t i = 0; i < x_.size(); ++i)
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
