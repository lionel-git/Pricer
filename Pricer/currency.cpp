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
		fxspot_ = invert_spot(1.0);
	}
	break;
	case currency_code::JPY:
	{
		exp_discount_rate_ = 0.6 / 100.0;
		fxspot_ = invert_spot(135.0);
	}
	break;
	default:
		throw std::runtime_error("Unkown ccy ?");
	}
}
