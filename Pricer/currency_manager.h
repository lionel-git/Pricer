#pragma once
#include "currency.h"
#include "amount.h"

#include <string>
#include <map>

class unit_tests;

class currency_manager
{
public:
	static const currency& get_currency(currency_code ccy);
	static const currency& get_currency(amount amt);

	friend unit_tests;

private:
	currency_manager(int setup_idx);
	currency_manager(const currency_manager&) = delete;
	currency_manager(const currency_manager&&) = delete;
	currency_manager& operator=(const currency_manager&) = delete;
	static const currency_manager& instance(int setup_idx);
	void add_currency(currency_code ccy, double discount_rate);

	std::map<currency_code, currency> currency_map_;

	int setup_idx_{ -1 };
};

