#include "brownian.h"
#include "pricer_exception.h"

#include <vector>
#include <cmath>
#include "my_format.h"

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

void
brownian::unit_test_cdf()
{
	static std::vector<std::pair<double, double>> test_cases =
	{
		// x  cdf(x)
		{ 0.0, 0.50000},
		{ 1.0, 0.84134},
		{ 2.0, 0.97725},
		{ 3.0, 0.99865},
		{ 4.0, 0.99997},
	};
	for (const auto& test : test_cases)
	{
		if (std::fabs(brownian::cdf(test.first) - test.second) > 1e-5)
			THROW(myfmt::format("Diff found: x={} {} vs {}", test.first, test.second, brownian::cdf(test.first)));
	}
}
