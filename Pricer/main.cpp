#include <iostream>
#include "currency_manager.h"
#include "fx_manager.h"
#include "fx_forward.h"
#include "fx_option.h"
#include "black_scholes.h"
#include "black_scholes2.h"
#include "normal.h"
#include "pricer_exception.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "brownian.h"
#include "unit_tests.h"
#include "lab.h"

#include <cfenv>

void test1()
{
    double expiry = 1.0;

    auto ccy_usd = currency_manager::get_currency(currency_code::USD);
    std::cout << ccy_usd.get_df(expiry) << std::endl;
    auto ccy_jpy = currency_manager::get_currency(currency_code::JPY);
    std::cout << ccy_jpy.get_df(expiry) << std::endl;
}

void test2()
{
    double expiry = 1.0;
    amount amount_asset(10'000, currency_code::USD);

    auto fxfwd = fx_forward(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
    std::cout << fxfwd.pv(fxfwd.base_currency()) << std::endl;

    double spot = fx_manager::get_fx(currency_code::USD, currency_code::JPY).get_spot();
    std::cout << spot << std::endl;

    double fwd = fx_manager::get_fx(currency_code::USD, currency_code::JPY).get_fwd(expiry);
    std::cout << fwd << std::endl;

    auto fxfwd2 = fx_forward(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, fwd));
    std::cout << fxfwd2.pv(fxfwd2.base_currency()) << std::endl;
}

void test3()
{
    double expiry = 1.0;
    amount amount_asset(10'000, currency_code::USD);

    auto fxo1 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
    std::cout << fxo1.pv(fxo1.base_currency()) << std::endl;

    double spot = fx_manager::get_fx(currency_code::USD, currency_code::JPY).get_spot();
    std::cout << spot << std::endl;

    double fwd = fx_manager::get_fx(currency_code::USD, currency_code::JPY).get_fwd(expiry);
    std::cout << fwd << std::endl;

    auto fxo2 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, fwd));
    std::cout << fxo2.pv(fxo2.base_currency()) << std::endl;
}

void test4()
{
    double expiry = 1.0;
    amount amount_asset(10'000, currency_code::USD);

    auto fxo1 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
    std::cout << fxo1.pv(fxo1.base_currency()) << std::endl;

    auto num_params_edp = numerical_parameters_edp(1000, 100, schema_type::EXPLICIT);
    auto model_edp = black_scholes(fxo1, num_params_edp);

    auto num_params_mc = numerical_parameters_mc(100, 10000);
    auto model_mc = black_scholes(fxo1, num_params_mc);

    std::cout << enumToText(model_mc.get_model_type()) << std::endl;
    std::cout << "Valo mc: " << model_mc.evaluate() << std::endl;

    auto model_normal_mc = normal(fxo1, num_params_mc);
    std::cout << enumToText(model_normal_mc.get_model_type()) << std::endl;
    std::cout << "Valo mc: " << model_normal_mc.evaluate() << std::endl;

    auto num_params_closed_f = numerical_parameters(0);
    auto model_closed_f = black_scholes(fxo1, num_params_closed_f);

    std::cout << enumToText(model_closed_f.get_model_type()) << std::endl;
    std::cout << "Valo closed f: " << model_closed_f.evaluate() << std::endl;

}

void test5()
{
    for (double x = -5.0; x <= 5.0; x += 0.05)
    {
        std::cout << x << ": " << brownian::cdf(x) << std::endl;
    }
}

void test6()
{
    double expiry = 1.0;
    amount amount_asset(10'000, currency_code::USD);
    auto fxo1 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
    std::cout << fxo1.pv(fxo1.base_currency()) << std::endl;

    auto num_params_closed_f = numerical_parameters(0);
    auto model_closed_f = black_scholes(fxo1, num_params_closed_f);
    std::cout << "Valo closed f: " << model_closed_f.evaluate() << std::endl;

    auto num_params_mc = numerical_parameters_mc(100, 10000);
    auto model_mc = black_scholes(fxo1, num_params_mc);
    std::cout << "Valo mc: " << model_mc.evaluate() << std::endl;

    auto num_params_edp = numerical_parameters_edp(1000, 50, schema_type::EXPLICIT);
    auto model_edp = black_scholes(fxo1, num_params_edp);
    std::cout << "Valo edp: " << model_edp.evaluate() << std::endl;

    auto num_params_edp_cn = numerical_parameters_edp(1000, 50, schema_type::CRANK_NICHOLSON);
    auto model_edp_cn = black_scholes(fxo1, num_params_edp_cn);
    model_edp_cn.set_theta(1.0); // Should match explicit
    std::cout << "Valo edp Theta= " << model_edp_cn.get_theta() << " : " << model_edp_cn.evaluate() << std::endl;
    model_edp_cn.set_theta(0.0); // implicit
    std::cout << "Valo edp Theta= " << model_edp_cn.get_theta() << " : " << model_edp_cn.evaluate() << std::endl;
    model_edp_cn.set_theta(0.5); // mixte
    std::cout << "Valo edp Theta= " << model_edp_cn.get_theta() << " : " << model_edp_cn.evaluate() << std::endl;
}

void test_unit()
{
    unit_tests::do_all_tests();
}

void test8()
{
    double expiry = 1.0;
    amount amount_asset(10'000, currency_code::USD);
    auto fxo1 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
    std::cout << fxo1.pv(fxo1.base_currency()) << std::endl;

    auto num_params_closed_f = numerical_parameters(0);
    auto model_closed_f = black_scholes(fxo1, num_params_closed_f);
    std::cout << "Valo closed f: " << model_closed_f.evaluate() << std::endl;

    auto num_params_mc = numerical_parameters_mc(100, 10000);
    auto model_mc = black_scholes(fxo1, num_params_mc);
    std::cout << "Valo mc: " << model_mc.evaluate() << std::endl;

    auto num_params_edp = numerical_parameters_edp(1000, 50, schema_type::EXPLICIT);
    auto model_edp = black_scholes(fxo1, num_params_edp);
    std::cout << "Valo edp explicit: " << model_edp.evaluate() << std::endl;

    auto num_params_edp_cn = numerical_parameters_edp(1000, 50, schema_type::CRANK_NICHOLSON);
    auto model_edp_cn = black_scholes(fxo1, num_params_edp_cn);
    model_edp_cn.set_theta(0.0); // implicit
    std::cout << "Valo edp implicit : " << model_edp_cn.evaluate() << std::endl;
}

void test_bs2()
{
    double expiry = 1.0;
    amount amount_asset(10'000, currency_code::USD);
    auto fxo1 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
    std::cout << fxo1.pv(fxo1.base_currency()) << std::endl;

    std::cout << "==== Testing BS2 =======" << std::endl;

    auto num_params_closed_f = numerical_parameters(0);
    auto model_closed_f = black_scholes2(fxo1, num_params_closed_f);
    std::cout << "Valo closed f: " << model_closed_f.evaluate() << std::endl;

    auto num_params_mc = numerical_parameters_mc(100, 10000);
    auto model_mc = black_scholes2(fxo1, num_params_mc);
    std::cout << "Valo mc: " << model_mc.evaluate() << std::endl;
}

int main(int /*argc*/, char** /*argv*/)
{
#ifdef _WIN32
    _controlfp_s(NULL, 0, _EM_ZERODIVIDE | _EM_INVALID | _EM_DENORMAL);
#endif

    try
    {
        //		test1();
        //		test5();
                //test2();
                //test3();
                //test4();
        //test_unit();
        //test6();
        //test_bs2();
        lab::test_integral();
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
