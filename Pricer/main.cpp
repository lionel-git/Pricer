#include <iostream>
#include "currency_manager.h"
#include "fx_manager.h"
#include "fx_forward.h"
#include "fx_option.h"


void test1()
{
	double expiry = 1.0;

	auto ccy_usd = currency_manager::get_currency(currency_code::USD);
	std::cout << ccy_usd.getDF(expiry) << std::endl;
	auto ccy_jpy = currency_manager::get_currency(currency_code::JPY);
	std::cout << ccy_jpy.getDF(expiry) << std::endl;
}

void test2()
{
	double expiry = 1.0;
	amount amount_asset(10'000, currency_code::USD);

	auto fxfwd = fx_forward(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, 100.0));
	std::cout << fxfwd.pv(fxfwd.base_currency()) << std::endl;

	double spot = fx_manager::get_fx(currency_code::USD, currency_code::JPY).getSpot();
	std::cout << spot << std::endl;

	double fwd = fx_manager::get_fx(currency_code::USD, currency_code::JPY).getFwd(expiry);
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

	double spot = fx_manager::get_fx(currency_code::USD, currency_code::JPY).getSpot();
	std::cout << spot << std::endl;

	double fwd = fx_manager::get_fx(currency_code::USD, currency_code::JPY).getFwd(expiry);
	std::cout << fwd << std::endl;

	auto fxo2 = fx_option(expiry, amount_asset, -amount_asset.strike_countervalue(currency_code::JPY, fwd));
	std::cout << fxo2.pv(fxo2.base_currency()) << std::endl;
}


int main(int /*argc*/, char** /*argv*/)
{
	try
	{
//		test1();
		test2();
		test3();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
