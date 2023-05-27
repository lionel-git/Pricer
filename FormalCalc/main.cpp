#include <iostream>
#include "known.h"
#include "unknown.h"

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Hello world" << std::endl;

		// Declare unknown variables
		unknown::start_registering(6);
		unknown Vim1("V[i-1]");
		unknown Vi("V[i]");
		unknown Vip1("V[i+1]");
		unknown::end_registering();


		// Declare known variables
		known r("r");
		known c1a("edp_coeffs_[i].c1a_");
		known c1b("edp_coeffs_[i].c1b_");
		known c1c("edp_coeffs_[i].c1c_");
		known xi("x[i]");
		known vol_bs("vol_bs_");

		auto value0 = r * Vi;
		auto value1 = r * xi;
		auto value2 = value1 * Vi;

		std::cout << "value2 = " << std::endl  << value2 << std::endl;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "SEH exception?" << std::endl;
	}
}

/*
EX of code :
	size_t N = V.size();
	std::vector<double> V2(N);²²
    V2[N-1] = V_up;
	V2[0] = V_down;
	for (int i = 1; i <= N - 2; ++i)
	{

		double value0 = r * V[i];
		double value1 = -r * x_[i] * (edp_coeffs_[i].c1a_ * V[i - 1] + edp_coeffs_[i].c1b_ * V[i] + edp_coeffs_[i].c1c_ * V[i + 1]);
		double value2 = -0.5 * vol_bs_ * vol_bs_ * x_[i] * x_[i] * (edp_coeffs_[i].c2a_ * V[i - 1] + edp_coeffs_[i].c2b_ * V[i] + edp_coeffs_[i].c2c_ * V[i + 1]);
		V2[i] = V[i] - dt * (value0 + value1 + value2);
	}
	V = V2;
*/

