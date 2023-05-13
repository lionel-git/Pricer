#include <iostream>
#include "currency_manager.h"
#include "fx_manager.h"
#include "fx_forward.h"
#include "fx_option.h"
#include "black_scholes.h"
#include "normal.h"
#include "pricer_exception.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"

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

	auto num_params_edp = numerical_parameters_edp(100, 0.0, 300.0, 1000);
	auto model_edp = black_scholes(fxo1, num_params_edp);

	auto num_params_mc = numerical_parameters_mc(100, 1000);
	auto model_mc = black_scholes(fxo1, num_params_mc);

	std::cout << enumToText(model_mc.get_model_type()) << std::endl;
	std::cout << "Valo mc: " << model_mc.evaluate() << std::endl;

	auto model_normal_mc = normal(fxo1, num_params_mc);
	std::cout << enumToText(model_normal_mc.get_model_type()) << std::endl;
	std::cout << "Valo mc: " << model_normal_mc.evaluate() << std::endl;

}

int main(int /*argc*/, char** /*argv*/)
{
	try
	{
//		test1();
		test2();
		test3();
		test4();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
