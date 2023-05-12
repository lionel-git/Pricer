#pragma once
#include <string>
#include "defines.h"

enum class currency_code { USD, JPY, EUR, HKD };
std::string enumToText(currency_code ccy);

class currency
{
public:
	double get_df(date_t t) const;

	friend class currency_manager;
private:
	currency(currency_code ccy, double discount_rate);

	currency_code ccy_;
	double exp_discount_rate_; // exponential annual rate for discount
};

