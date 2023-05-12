#pragma once
#include "numerical_parameters.h"

class numerical_parameters_mc : public numerical_parameters
{
public:
	numerical_parameters_mc(int time_points, int simuls);
	virtual numerical_method get_numerical_method() const override { return numerical_method::MC; }

public:
	int simuls_;
};
