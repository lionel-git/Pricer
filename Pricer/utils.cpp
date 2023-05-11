#include "utils.h"

double check_sign(double x)
{
	if (x > 0.0)
		return 1.0;
	if (x < 0.0)
		return -1.0;
	THROW("value is 0.0");
}

void check_opposite_signs(double a, double b)
{
	if (a * b >= 0)
		THROW("values have same sign");
}

double relu(double x)
{
	if (x <= 0)
		return 0.0;
	return x;
}
