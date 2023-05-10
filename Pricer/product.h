#pragma once
#include "amount.h"

class product
{
public:
	product(date_t expiry, amount base_amount);
	virtual ~product() = default;

	currency_code base_currency() const { return base_amount_.currency_; }
	virtual amount pv(currency_code target_ccy) const;

protected:
	date_t expiry_;
	amount base_amount_;
	const currency& base_ccy_;
};
