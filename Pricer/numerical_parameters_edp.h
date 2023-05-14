#pragma once
#include "numerical_parameters.h"

enum class schema_type { EXPLICIT, IMPLICIT, CRANK_NICHOLSON };

class numerical_parameters_edp : public numerical_parameters
{
public:
	numerical_parameters_edp(int time_points, int x_points, schema_type schema);
	virtual numerical_method get_numerical_method() const override { return numerical_method::EDP; }

public:
	int x_points_;
	schema_type schema_;
};

