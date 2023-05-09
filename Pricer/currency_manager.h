#pragma once
#include "currency.h"
#include "amount.h"

#include <string>
#include <map>

class currency_manager
{
public:
	static const currency_manager& instance();

	static const currency& get_currency(currency_code ccy);
	static const currency& get_currency(amount amt);

	static double get_fxspot(currency_code asset, currency_code basis);
	static double get_fxspot(amount asset, amount basis);
	static double get_fxfwd(date_t expiry, currency_code asset, currency_code basis);
	static double get_fxfwd(date_t expiry, amount asset, amount basis);

private:
	currency_manager();
	currency_manager(const currency_manager&) = delete;
	currency_manager(const currency_manager&&) = delete;

	std::map<currency_code, currency> currency_map_;
};

