#include "black_scholes.h"
#include "fx_manager.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "brownian.h"
#include <cmath>

black_scholes::black_scholes(const product& product, const numerical_parameters& np) :
	model(product, np), vol_bs_(product_.get_fx().get_bs_vol())
{
	initialize();
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

	if (product_.get_sign() > 0)
		return (brownian::cdf(d_plus) * F - brownian::cdf(d_minus) * K) * basis_.get_df(T); // call
	else if (product_.get_sign() < 0)
		return (brownian::cdf(-d_minus) * K - brownian::cdf(-d_plus) * F) * basis_.get_df(T); // put
	THROW("Bad sign");
}

void 
black_scholes::get_edp_xbounds(double& x_min, double& x_max) const
{
	double T = product_.get_expiry();
	double F = product_.get_fx().get_fwd(T);
	double vol_time = vol_bs_ * sqrt(T);
	x_max = F + 3 * F * vol_time;
	x_min = 0.0;
}

void
black_scholes::back_propagate_explicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/) const
{
	// EDP: dV/dt + 0.5*s^2*d2V/dS2 + r.S.dV/dS - r.V = 0
	// dV/dt = r.V - 0.5*s^2*d2V/dS2 - r.S.dV/dS
	// V(t+dt)-V(t) = dt.(r.V - 0.5*s^2*d2V/dS2 - r.S.dV/dS )


}

void
black_scholes::back_propagate_implicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/) const
{


}

void
black_scholes::back_propagate_cranck_nicholson(std::vector<double>& /*V*/, double /*dt*/, double /*r*/) const
{


}

double
black_scholes::get_dS_mc(double normal_value, double S, double dt) const
{
	// dS = S.s.dWt
	return S * vol_bs_ * normal_value * sqrt(dt);
}
