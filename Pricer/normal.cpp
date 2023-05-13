#include "normal.h"

normal::normal(const product& product, const numerical_parameters& np) :
	model(product, np)
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
normal::evaluate_mc() const
{
	return 0.0;
}

