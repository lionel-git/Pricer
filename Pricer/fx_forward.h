#pragma once
#include "product.h"
#include "amount.h"

class fx_forward : public product
{
public:
	fx_forward(date_t expiry, amount asset_amount, amount base_amount);

	virtual amount pv() const;

protected:
	amount asset_amount_;
	const currency& asset_ccy_;
};
