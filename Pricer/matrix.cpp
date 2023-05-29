#include "matrix.h"
#include "pricer_exception.h"
#include <format>

// n = vector size
// a: 2 a n-1
// b: 1 a n-1
// c: 1 a n-2
// r: 1 a n-1
// out : 1 a n-1
std::vector<double>
matrix::solve_tridiagonal_system(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c, const std::vector<double>& r)
{
	size_t n = a.size();

	// g[i]: upper diag
	// p[i]: new target vecteur
	// x: output vector
	std::vector<double> g(n); // 1 a n-2
	std::vector<double> p(n); // 1 a n-1
	std::vector<double> x(n); // 1 a n-1

	// First stage
	g[1] = c[1] / b[1];
	p[1] = r[1] / b[1];
	for (size_t i = 2; i <= n - 2; ++i)
	{
		g[i] = c[i] / (b[i] - a[i] * g[i - 1]);
		p[i] = (r[i] - a[i] * p[i - 1]) / (b[i] - a[i] * g[i - 1]);
	}
	p[n - 1] = (r[n - 1] - a[n - 1] * p[n - 2]) / (b[n - 1] - a[n - 1] * g[n - 2]);

	// Second stage
	x[n - 1] = p[n - 1];
	for (size_t i = n - 2; i >= 1; --i)
		x[i] = p[i] - g[i] * x[i + 1];
	return x;
}

void 
matrix::unit_test_solve_tridiagonal_system()
{
	// 4x4 matrix
	std::vector<double> a = { 0, 0, 3, 6,  9 };
	std::vector<double> b = { 0, 1, 4, 7, 10 };
	std::vector<double> c = { 0, 2, 5, 8,  0 };
	std::vector<double> r = { 0, -5, -8, -17, 7 };

	auto x = solve_tridiagonal_system(a, b, c, r);
	for (int i = 1; i <= 4; ++i)
	{
		double v;
		if (i == 1)
			v = b[i] * x[i] + c[i] * x[i + 1];
		else if (i == 4)
			v = a[i] * x[i - 1] + b[i] * x[i];
		else
			v = a[i] * x[i - 1] + b[i] * x[i] + c[i] * x[i + 1];
		if (fabs(v - r[i]) > 1e-8)
			THROW(std::format("diff at i={}", i));
	}
}
