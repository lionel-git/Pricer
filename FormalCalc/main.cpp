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
		// U represente le vecteur a t: inconnu
		const unknown Uim1("U[i-1]");
		const unknown Ui("U[i]");
		const unknown Uip1("U[i+1]");
		unknown::end_registering();

		// V represente le vecteur a t+dt: connu
		const known Vim1("V[i-1]");
		const known Vi("V[i]");
		const known Vip1("V[i+1]");

		// Declare known variables
		const known r("r");
		const known dt("dt");
		const known c1a("c1a");
		const known c1b("c1b");
		const known c1c("c1c");
		const known c2a("c2a");
		const known c2b("c2b");
		const known c2c("c2c");
		const known xi("x[i]");
		const known vol_bs("vol_bs");
		const known theta("theta");

		/*auto vv = Vi + known(5);
		std::cout << "vv = " << std::endl << vv << std::endl;
		return 1;*/

		auto v0 = r * Vi;
		auto v1 = -r * xi * (c1a * Vim1 + c1b * Vi + c1c * Vip1);
		auto v2 = -known(0.5) * vol_bs * vol_bs * xi * xi * (c2a * Vim1 + c2b * Vi + c2c * Vip1);
		auto res_explicit = Vi - dt * (v0 + v1 + v2);
		std::cout << "res0 = " << std::endl << res_explicit << std::endl;

		auto u0 = r * Ui;
		auto u1 = -r * xi * (c1a * Uim1 + c1b * Ui + c1c * Uip1);
		auto u2 = -known(0.5) * vol_bs * vol_bs * xi * xi * (c2a * Uim1 + c2b * Ui + c2c * Uip1);
		auto res_implicit = Ui - dt * (u0 + u1 + u2);
		std::cout << "res1 = " << std::endl << res_implicit << std::endl;

		// theta=1 => explicit
		auto res_zero = theta * res_explicit + (known::One - theta) * res_implicit - Ui;
		std::cout << "res_zero = " << std::endl << res_zero << std::endl;

		// Faire les cas limite up/down (0, N-1)
		// Regarder resolution matrice tri diagonale (5 diag ?)
		// decomp LU ?



		
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

