#include "black_scholes.h"

black_scholes::black_scholes(const product& product, const numerical_parameters& np):
	model(product, np), vol_bs_(product_.get_fx().get_bs_vol())
{
}

double 
black_scholes::evaluate_closed_f() const
{


	return 0.0;
}

double 
black_scholes::evaluate_edp() const
{
	return 0.0;
}

double 
black_scholes::evaluate_mc() const
{




	return 0.0;
}
