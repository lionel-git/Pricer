#include "currency.h"
#include "pricer_exception.h"
#include <cmath>

std::string
enumToText(currency_code ccy)
{
	switch (ccy)
	{
	case currency_code::USD:
		return "USD";
	case currency_code::JPY:
		return "JPY";
	case currency_code::EUR:
		return "EUR";
	case currency_code::HKD:
		return "HKD";
	default:
		THROW("Unkown ccy ?");
	}
}

double
currency::get_df(date_t t) const
{
	return std::exp(-exp_discount_rate_ * t);
}

currency::currency(currency_code ccy, double discount_rate): 
	ccy_(ccy), exp_discount_rate_(discount_rate)
{
}
