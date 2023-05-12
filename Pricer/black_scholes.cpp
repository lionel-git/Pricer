#include "black_scholes.h"


black_scholes::black_scholes(const product& product, const numerical_parameters& np):
	model(product, np)
{
}
