#pragma once
#include "numerical_parameters.h"

class numerical_parameters_edp : public numerical_parameters
{
public:
	numerical_parameters_edp(int time_points, double x_min, double x_max, int x_points);
	virtual numerical_method get_numerical_method() const override { return numerical_method::EDP; }

public:
	double x_min_;
	double x_max_;
	int x_points_;
};

