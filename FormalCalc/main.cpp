#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "Hello world" << std::endl;
}

/*
EX of code :
	size_t N = V.size();
	std::vector<double> V2(N);
	// The first and last values are unchanged
	V2[0] = V[0];
	V2[N - 1] = V[N - 1] - dt * r * (V[N - 1] - x_[N - 1]); // mieux gerer techniquement les valurs aux bornes + valeurs ?
	for (int i = 1; i <= N - 2; ++i)
	{

		double value0 = r * V[i];
		double value1 = -r * x_[i] * (edp_coeffs_[i].c1a_ * V[i - 1] + edp_coeffs_[i].c1b_ * V[i] + edp_coeffs_[i].c1c_ * V[i + 1]);
		double value2 = -0.5 * vol_bs_ * vol_bs_ * x_[i] * x_[i] * (edp_coeffs_[i].c2a_ * V[i - 1] + edp_coeffs_[i].c2b_ * V[i] + edp_coeffs_[i].c2c_ * V[i + 1]);
		V2[i] = V[i] - dt * (value0 + value1 + value2);
	}
	V = V2;

*/
