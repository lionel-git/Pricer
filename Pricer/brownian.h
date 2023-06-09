#pragma once
#include <random>

class brownian
{
public:
	brownian();
	double normal_value();
	static double cdf(double x);

	static void unit_test_cdf();

private:
	std::default_random_engine generator_; // mersenne twister by default
	std::normal_distribution<double> distribution_;
};

