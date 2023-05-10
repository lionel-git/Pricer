#pragma once
#include "currency.h"
#include "amount.h"

#include <string>
#include <map>

class currency_manager
{
public:
	static const currency& get_currency(currency_code ccy);
	static const currency& get_currency(amount amt);

private:
	currency_manager();
	currency_manager(const currency_manager&) = delete;
	currency_manager(const currency_manager&&) = delete;
	currency_manager& operator=(const currency_manager&) = delete;
	static const currency_manager& instance();
	void add_currency(currency_code ccy, double discount_rate);

	std::map<currency_code, currency> currency_map_;
};

