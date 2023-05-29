#pragma once
#include <vector>

class matrix
{
public:
	static std::vector<double> solve_tridiagonal_system(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c, const std::vector<double>& r);

	static void unit_test_solve_tridiagonal_system();
};

