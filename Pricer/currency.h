#pragma once
#include <string>
#include "defines.h"

enum class currency_code { USD, JPY };
std::string enumToText(currency_code ccy);

class currency
{
public:
	double getDF(date_t t) const;

	friend class currency_manager;
private:
	currency(currency_code ccy);

	double exp_discount_rate_; // exponential annual rate for discount
	double fxspot_;            // fxspot USDXXX (how many XXX for 1USD)
};

