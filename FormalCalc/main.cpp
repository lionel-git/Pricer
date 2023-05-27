#include <iostream>
#include "known.h"
#include "unknown.h"

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Hello world" << std::endl;

		// Declare unknown variables
		unknown::start_registering(3);
		const unknown Vim1("V[i-1]");
		const unknown Vi("V[i]");
		const unknown Vip1("V[i+1]");
		unknown::end_registering();

		// Declare known variables
		known r("r");
		known dt("dt");
		known c1a("edp_coeffs_[i].c1a_");
		known c1b("edp_coeffs_[i].c1b_");
		known c1c("edp_coeffs_[i].c1c_");
		known c2a("edp_coeffs_[i].c2a_");
		known c2b("edp_coeffs_[i].c2b_");
		known c2c("edp_coeffs_[i].c2c_");
		known xi("x[i]");
		known vol_bs("vol_bs_");

		/*auto vv = Vi + known(5);
		std::cout << "vv = " << std::endl << vv << std::endl;
		return 1;*/

		auto value0 = r * Vi;
		auto value1 = -r * xi * (c1a * Vim1 + c1b * Vi + c1c * Vip1);
		auto value2 = -known(0.5) * vol_bs * vol_bs * xi * xi * (c2a * Vim1 + c2b * Vi + c2c * Vip1);
		auto res = Vi - dt * (value0 + value1 + value2);
		std::cout << "res = " << std::endl << res << std::endl;

		
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

