#include "model.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "pricer_exception.h"
#include "black_scholes.h"
#include "normal.h"

std::string 
enumToText(model_type mt)
{
	switch (mt)
	{
	case model_type::BLACK_SCHOLES:
		return "BS";
	case model_type::NORMAL:
		return "NORMAL";
	default:
		THROW("Unkown model type?");
	}

}

model::model(const product& product, const numerical_parameters& np) : 
	product_(product), numerical_parameters_(np), asset_(product.get_fx().get_asset()), basis_(product.get_fx().get_basis())
{
	initialize();
}

static
void initialize_points(std::vector<double>& v, double min, double max, int nb_points)
{
	if (nb_points < 2)
		THROW("Not enough points");
	v.resize(nb_points);
	double dx = (max - min) / (v.size() - 1);
	for (int i = 0; i < v.size(); ++i)
		v[i] = min + i * dx; // x0=min, x_end=max
}

void
model::initialize_common()
{
	initialize_points(t_, 0.0, product_.get_expiry(), numerical_parameters_.time_points_);
	
	// On a: S(t) = S0.Dfa(t)/Dfb(t)
	// On a: S(t+dt)/S(t) = [Dfa(t+dt)/Dfb(t+dt)] / [Dfa(t)/Dfb(t)] = q(t) 
	// On a: S(t+dt)  = S(t).q(t)
	// dS = S(t+dt)-S(t) = S(t).(q(t)-1) = S(t).r(t) avec r(t)=q(t)-1
	//
	// Si Df(t)=exp(-r.t)
	// alors S(t) = S0.exp((-ra+rb).t)
	// alors S(t+dt) = S(t).exp((-ra+rb).dt)

	r_.resize(t_.size());
	double prev_ratio = 1.0;
	r_[0] = 0.0;
	for (int i = 1; i < r_.size(); ++i)
	{
		double ratio = asset_.get_df(t_[i]) / basis_.get_df(t_[i]);
		r_[i] = ratio / prev_ratio - 1.0;
		prev_ratio = ratio;
	}
}

void 
model::initialize_edp()
{
	initialize_common();
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	initialize_points(x_, params.x_min_, params.x_max_, params.x_points_);
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
	for (int simul = 0; simul < params.simuls_; ++simul)
	{
		double St = product_.get_fx().get_spot();
		for (int i = 0; i < t_.size() - 1; ++i)
		{
			double dt = t_[i + 1] - t_[i];
			// on diffuse le spot entre t[i] et t[i+1]
			double dS_deter = St * r_[i + 1];
			double dS_stochastic = get_dS_mc(St, dt);
			St += dS_deter + dS_stochastic;
		}
		sum_payoffs += product_.payoff(St);
	}
	return (sum_payoffs / params.simuls_) * basis_.get_df(product_.get_expiry());
}

