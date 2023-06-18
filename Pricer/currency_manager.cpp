#include "currency_manager.h"
#include "pricer_exception.h"

#include <format>
#include <iostream>

const currency_manager& 
currency_manager::instance(int setup_idx)
{
	static const currency_manager manager(setup_idx);
	if (setup_idx != 0 && setup_idx != manager.setup_idx_)
		THROW(std::format("Change of setup: {} => {}", manager.setup_idx_, setup_idx));
	return manager;
}

const currency& 
currency_manager::get_currency(currency_code ccy)
{
	const auto& manager = instance(0);
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

currency_manager::currency_manager(int setup_idx)
{
	std::cout << "Init currency_manager with setup: " << setup_idx << std::endl;
	switch (setup_idx)
	{
	case 0:
		add_currency(currency_code::USD, 0.6 / 100.0);
		add_currency(currency_code::JPY, 0.6 / 100.0);
		add_currency(currency_code::EUR, 2.9 / 100.0);
		add_currency(currency_code::HKD, 3.1 / 100.0);
		break;
	case 1:
		add_currency(currency_code::USD, 3.5 / 100.0);
		add_currency(currency_code::JPY, 0.6 / 100.0);
		add_currency(currency_code::EUR, 2.9 / 100.0);
		add_currency(currency_code::HKD, 3.1 / 100.0);
		break;
	default:
		THROW(std::format("Invalid setup_idx: {}", setup_idx));
	}
	setup_idx_ = setup_idx;
}
