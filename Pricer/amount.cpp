#include "amount.h"
#include "fx_manager.h"
#include "pricer_exception.h"

amount::amount(double notional, currency_code ccy) :
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
		THROW("Different currencies!");
	return amount(notional_ + value.notional_, currency_);
}

amount
amount::operator-() const
{
	return amount(-notional_, currency_);
}

amount
amount::spot_countervalue(currency_code target_ccy) const
{
	double spot = 1.0;
	if (target_ccy != currency_)
		spot = fx_manager::get_fx(currency_, target_ccy).get_spot();
	return amount(notional_ * spot, target_ccy);
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
