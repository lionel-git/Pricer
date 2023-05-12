#include "numerical_parameters_edp.h"

numerical_parameters_edp::numerical_parameters_edp(int time_points, double x_min, double x_max, int x_points):
	numerical_parameters(time_points), x_min_(x_min), x_max_(x_max), x_points_(x_points)
{
}
