#include "normal.h"
#include "brownian.h"

normal::normal(const product& product, const numerical_parameters& np) :
	model(product, np), vol_n_(product_.get_fx().get_normal_vol())
{
	initialize();
	check_parameters();
}

void
normal::check_parameters() const
{
	if (vol_n_ < -1e-10)
		THROW("Vol cannot be negative");
}

double
normal::evaluate_closed_f() const
{
	return 0.0;

}



void
normal::get_edp_xbounds(double& x_min, double& x_max) const
{
	double T = product_.get_expiry();
	double F = product_.get_fx().get_fwd(T);
	double vol_time = vol_n_ * sqrt(T);
	x_max = std::max(F + 5 * vol_time, product_.get_fx().get_spot() * 1.1);
	x_min = std::min(F - 5 * vol_time, product_.get_fx().get_spot() * 0.9);
}

double 
normal::get_dS_mc(double normal_value, double /*S*/, double dt) const
{
	// dS = s.dWt
	return vol_n_ * normal_value * sqrt(dt);
}
