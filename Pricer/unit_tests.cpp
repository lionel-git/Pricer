#include "unit_tests.h"
#include "matrix.h"
#include "brownian.h"
#include "currency_manager.h"
#include "fx_manager.h"

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
        std::cerr << "failed: " << "SEH exception?" << std::endl;
    }
}

void
unit_tests::configure_market_data()
{
    currency_manager::instance(0);
    fx_manager::instance(0);
}

void
unit_tests::do_all_tests()
{
    std::cout << " ==== Start unit tests ====" << std::endl;
    configure_market_data();
    execute_test(matrix::unit_test_solve_tridiagonal_system, "matrix::unit_test_solve_tridiagonal_system");
    execute_test(brownian::unit_test_cdf, "brownian::unit_test_cdf");
    std::cout << " ==== End unit tests ====" << std::endl;
}

