#include "currency_manager.h"
#include "pricer_exception.h"

const currency_manager& 
currency_manager::instance()
{
	static const currency_manager manager;
	return manager;
}

const currency& 
currency_manager::get_currency(currency_code ccy)
{
	const auto& manager = instance();
	auto it = manager.currency_map_.find(ccy);
	if (it == manager.currency_map_.end())
		THROW("cannot find cur: " + enumToText(ccy));
	return it->second;
}

const currency& 
currency_manager::get_currency(amount amt)
{
	return get_currency(amt.currency_);
}


void 
currency_manager::add_currency(currency_code ccy, double discount_rate)
{
	currency_map_.emplace(ccy, currency(ccy, discount_rate));
}

currency_manager::currency_manager()
{
	add_currency(currency_code::USD, 3.5 / 100.0);
	add_currency(currency_code::JPY, 0.6 / 100.0);
	add_currency(currency_code::EUR, 2.9 / 100.0);
	add_currency(currency_code::HKD, 3.1 / 100.0);
}

