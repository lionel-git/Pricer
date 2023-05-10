#include "amount.h"
#include "fx_manager.h"
#include <stdexcept>

amount::amount(double notional, currency_code ccy): 
	notional_(notional), currency_(ccy)
{
}

amount
amount::operator*(double mult) const
{
	return amount(mult * notional_, currency_);
}

amount 
amount::operator+(amount value) const
{
	if (currency_ != value.currency_)
		throw std::runtime_error("Different currencies!");
	return amount(notional_ + value.notional_, currency_);
}

amount 
amount::operator-() const
{
	return amount(-notional_, currency_);
}

amount 
amount::countervalue(currency_code target_ccy) const
{
	return amount(notional_* fx_manager::get_fx(currency_, target_ccy).getSpot(), target_ccy);
}

amount 
amount::strike_countervalue(currency_code target_ccy, double strike) const
{
	return amount(notional_ * strike, target_ccy);
}

std::ostream& operator<<(std::ostream& os, const amount& amt)
{
	os << amt.notional_ << " " << enumToText(amt.currency_);
	return os;
}
