#include <iostream>
#include "currency_manager.h"
#include "fx_manager.h"
#include "fx_forward.h"

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

	auto fxfwd = fx_forward(expiry, amount_asset, -amount_asset.strike_countervalue(100.0, currency_code::JPY));
	std::cout << fxfwd.pv() << std::endl;

	double spot = fx_manager::get_fx(currency_code::USD, currency_code::JPY).getSpot();
	std::cout << spot << std::endl;

	double fwd = fx_manager::get_fx(currency_code::USD, currency_code::JPY).getFwd(expiry);
	std::cout << fwd << std::endl;

	auto fxfwd2 = fx_forward(expiry, amount_asset, -amount_asset.strike_countervalue(fwd, currency_code::JPY));
	std::cout << fxfwd2.pv() << std::endl;
}

int main(int /*argc*/, char** /*argv*/)
{
	try
	{
		test2();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
