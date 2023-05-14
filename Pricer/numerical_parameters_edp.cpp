#include "numerical_parameters_edp.h"

numerical_parameters_edp::numerical_parameters_edp(int time_points, int x_points, schema_type schema):
	numerical_parameters(time_points), x_points_(x_points), schema_(schema)
{
}
