#pragma once
#include "amount.h"

class product
{
public:
	product(date_t expiry, amount base_amount);
	virtual ~product() = default;

	virtual amount pv() const;

protected:
	date_t expiry_;
	amount base_amount_;
	const currency& base_ccy_;
};
