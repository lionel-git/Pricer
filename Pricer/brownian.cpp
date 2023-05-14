#include "brownian.h"

brownian::brownian():
	generator_(), distribution_(0.0, 1.0) // mean, stddev
{
}

double 
brownian::normal_value()
{
	return distribution_(generator_);
}

double 
brownian::cdf(double x)
{
	return 0.5 * std::erfc(-x * sqrt(0.5));
}
