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
	x_max = std::max(F + 3 * F * vol_time, product_.get_fx().get_spot() * 1.1);
	x_min = 0.0;
}

// input: V at t+dt
void
black_scholes::back_propagate_explicit(std::vector<double>& V, double dt, double r) const
{
	// EDP: dV/dt + 0.5*s^2*d2V/dS2 + r.S.dV/dS - r.V = 0
	// dV/dt = r.V - 0.5*s^2*d2V/dS2 - r.S.dV/dS
	// V(t+dt)-V(t) = dt.(r.V - 0.5*s^2*d2V/dS2 - r.S.dV/dS )
	// V(t) = V(t+dt) - dt.(r.V - r.S.dV/dS - 0.5*s^2*d2V/dS2 )

	// Rem S=x[i], V and x have same length

	size_t N = V.size();
	std::vector<double> V2(N);
	// The first and last values are unchanged
	V2[0] = V[0];
	V2[N - 1] = V[N - 1];
	for (int i = 1; i <= N - 2; ++i)
	{
		
		double value0 = r * V[i];
		double value1 = -r * x_[i] * (edp_coeffs_[i].c1a_ * V[i - 1] + edp_coeffs_[i].c1b_ * V[i] + edp_coeffs_[i].c1c_ * V[i + 1]);
		double value2 = -0.5 * vol_bs_ * vol_bs_ * (edp_coeffs_[i].c2a_ * V[i - 1] + edp_coeffs_[i].c2b_ * V[i] + edp_coeffs_[i].c2c_ * V[i + 1]);
		V2[i] = V[i] - dt * (value0 + value1 + value2);
	}
	V = V2;
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
