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
