#include "currency.h"
#include <stdexcept>
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
		throw std::runtime_error("Unkown ccy ?");
	}
}

double
currency::getDF(date_t t) const
{
	return std::exp(-exp_discount_rate_ * t);
}

// Just to invert spot 
static 
double invert_spot(double fx)
{
	return 1.0 / fx;
}

currency::currency(currency_code ccy)
{
	switch (ccy)
	{
	case currency_code::USD:
	{
		exp_discount_rate_ = 3.5 / 100.0;
		fxspot_ = invert_spot(1.0); // USDUSD
	}
	break;
	case currency_code::JPY:
	{
		exp_discount_rate_ = 0.6 / 100.0;
		fxspot_ = invert_spot(135.0); // USDJPY
	}
	break;
	case currency_code::EUR:
	{
		exp_discount_rate_ = 2.9 / 100.0;
		fxspot_ = invert_spot(0.91); // USDEUR
	}
	break;
	case currency_code::HKD:
	{
		exp_discount_rate_ = 3.1 / 100.0;
		fxspot_ = invert_spot(7.84); // USDKHD
	}
	break;
	default:
		throw std::runtime_error("Unkown ccy ?");
	}
}
