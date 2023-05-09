#include "amount.h"
#include "currency_manager.h"
#include <stdexcept>

amount::amount(double notional, currency_code ccy): notional_(notional), currency_(ccy)
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
amount::countervalue(currency_code ccy) const
{
	auto spot = currency_manager::get_fxspot(currency_, ccy);
	return amount(notional_ * spot, ccy);
}

amount 
amount::strike_countervalue(double strike, currency_code ccy) const
{
	return amount(notional_ * strike, ccy);
}

std::ostream& operator<<(std::ostream& os, const amount& amt)
{
	os << amt.notional_ << " " << enumToText(amt.currency_);
	return os;
}
