#include "normal.h"
#include "brownian.h"

normal::normal(const product& product, const numerical_parameters& np) :
	model(product, np), vol_n_(product_.get_fx().get_normal_vol())
{
}

double
normal::evaluate_closed_f() const
{
	return 0.0;

}

double 
normal::evaluate_edp() const
{
	return 0.0;

}

double 
normal::get_dS_mc(double normal_value, double /*S*/, double dt) const
{
	// dS = s.dWt
	return vol_n_ * normal_value * sqrt(dt);
}
