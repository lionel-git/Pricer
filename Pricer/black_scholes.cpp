#include "black_scholes.h"
#include "fx_manager.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "brownian.h"
#include <cmath>

black_scholes::black_scholes(const product& product, const numerical_parameters& np) :
	model(product, np), vol_bs_(product_.get_fx().get_bs_vol())
{
}

double
black_scholes::evaluate_closed_f() const
{
	double T = product_.get_expiry();
	double F = product_.get_fx().get_fwd(T);
	double K = product_.get_strike();
	double vol_time = vol_bs_ * sqrt(T);

	double d_plus = std::log(F / K) / vol_time + 0.5 * vol_time;
	double d_minus = d_plus - vol_time;

	return (brownian::cdf(d_plus) - brownian::cdf(d_minus)) * basis_.get_df(T);
}

double
black_scholes::evaluate_edp() const
{
	return 0.0;
}

double
black_scholes::get_dS_mc(double normal_value, double S, double dt) const
{
	// dS = S.s.dWt
	return S * vol_bs_ * normal_value * sqrt(dt);
}
