#include "unit_tests.h"
#include "matrix.h"
#include "brownian.h"

#include <iostream>

static 
void execute_test(void (*test_func)(void), const std::string& name)
{
	try
	{
		std::cout << "Start test: " << name << "... ";
		test_func();
		std::cout << "passed" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "failed: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "failed: " <<  "SEH exception?" << std::endl;
	}
}

void 
unit_tests::do_all_tests()
{
	execute_test(matrix::unit_test_solve_tridiagonal_system, "matrix::unit_test_solve_tridiagonal_system");
	execute_test(brownian::unit_test_cdf, "brownian::unit_test_cdf");
}

