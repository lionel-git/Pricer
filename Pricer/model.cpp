#include "model.h"

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

void
model::initialize_grid_axis(std::vector<double>& v, double min, double max, int nb_points, const std::set<double>& critical_points, double percent_dx)
{
	if (nb_points < 2)
		THROW("Not enough points");
	double dx = (max - min) / (nb_points - 1);
	std::set<double> s;
	for (int i = 0; i < nb_points; ++i)
	{
		double x = min + i * dx;
		if (!is_near_critical_point(x, critical_points, dx * percent_dx))
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
	initialize_grid_axis(t_, 0.0, product_.get_expiry(), numerical_parameters_.time_points_, product_.get_critical_times(), eps_percent_dt_);

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
