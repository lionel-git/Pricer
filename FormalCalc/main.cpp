#include <iostream>
#include "known.h"
#include "unknown.h"

enum class TestCase { Down, Regular, Up};
std::string enumToText(TestCase test_case);

std::string enumToText(TestCase test_case)
{
	switch (test_case)
	{
	case TestCase::Down:
		return "Down";
	case TestCase::Regular:
		return "Regular";
	case TestCase::Up:
		return "Up";
	default:
		throw std::runtime_error("Bad case");
	}
}

void generate_formula(TestCase test_case)
{
	//std::cout << "Test case: " << enumToText(test_case) << std::endl;

	// Declare known variables
	const known r("r");
	const known dt("dt");
	const known c1a("c1a");
	const known c1b("c1b");
	const known c1c("c1c");
	const known c2a("c2a");
	const known c2b("c2b");
	const known c2c("c2c");
	const known xi("x_[i]");
	const known vol_bs("vol_bs_");
	const known theta("theta_");

	// V represente le vecteur a t+dt: connu
	const known Vim1("V[i-1]");
	const known Vi("V[i]");
	const known Vip1("V[i+1]");
	// U_up/U_down (connu a t)
	const known U_up("U_up"); 
	const known U_down("U_down");


	// Declare unknown variables
	unknown::start_registering(3);
	// U represente le vecteur a t: inconnu
	unknown Uim1("U[i-1]");
	const unknown Ui("U[i]");
    unknown Uip1("U[i+1]");
	unknown::end_registering();

	if (test_case == TestCase::Down)
		Uim1 = U_down;

	if (test_case == TestCase::Up)
		Uip1 = U_up;

	auto v0 = r * Vi;
	auto v1 = -r * xi * (c1a * Vim1 + c1b * Vi + c1c * Vip1);
	auto v2 = -known(0.5) * vol_bs * vol_bs * xi * xi * (c2a * Vim1 + c2b * Vi + c2c * Vip1);
	auto res_explicit = Vi - dt * (v0 + v1 + v2);
	//std::cout << "res0 = " << std::endl << res_explicit << std::endl;

	auto u0 = r * Ui;
	auto u1 = -r * xi * (c1a * Uim1 + c1b * Ui + c1c * Uip1);
	auto u2 = -known(0.5) * vol_bs * vol_bs * xi * xi * (c2a * Uim1 + c2b * Ui + c2c * Uip1);
	auto res_implicit = Ui - dt * (u0 + u1 + u2);
	//std::cout << "res1 = " << std::endl << res_implicit << std::endl;

	// theta=1 => explicit
	auto res_zero = theta * res_explicit + (known::One - theta) * res_implicit - Ui;
	//std::cout << "0 = SUM" << std::endl << res_zero << std::endl << std::endl;
	//std::cout << "==========" << std::endl;

	static std::vector<std::string> aliases =
	{
		"double c1a = edp_coeffs_[i].c1a_;",
		"double c1b = edp_coeffs_[i].c1b_;",
		"double c1c = edp_coeffs_[i].c1c_;",
		"double c2a = edp_coeffs_[i].c2a_;",
		"double c2b = edp_coeffs_[i].c2b_;",
		"double c2c = edp_coeffs_[i].c2c_;"
	};

	std::vector<std::string> start_code = { "{" };
	std::vector<std::string> end_code = { "}" };
	switch (test_case)
	{
	case TestCase::Down:
		start_code.push_back("size_t i = 1;");		
		break;
	case TestCase::Regular:
		start_code.push_back("for (size_t i=2 ; i < N-2 ; ++i) {");
		end_code.push_back("}");
		break;
	case TestCase::Up:
		start_code.push_back("size_t i = N-2;");
		break;
	}
	start_code.insert(start_code.end(), aliases.begin(), aliases.end());
		
	std::cout << res_zero.get_code(start_code, end_code) << std::endl;
}

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Hello world" << std::endl;
		std::cout << "// ========= GENERATED CODE ===========" << std::endl;
		generate_formula(TestCase::Down);
		generate_formula(TestCase::Regular);
		generate_formula(TestCase::Up);
		std::cout << "// ========= END GENERATED CODE ===========" << std::endl;

		// regarder comment resoudre tri diag system



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
