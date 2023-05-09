#pragma once
#include "currency.h"
#include <ostream>

struct amount
{
	amount(double notional, currency_code ccy);

	amount operator*(double mult) const;
	amount operator+(amount value) const;
	amount operator-() const;

	amount countervalue(currency_code ccy) const;

	amount strike_countervalue(double strike, currency_code ccy) const;


	// Notional is signed!
	double notional_;
	currency_code currency_;

	friend std::ostream& operator<<(std::ostream& os, const amount& amt);
};
