#pragma once
#include "product.h"
#include "amount.h"
#include "fx.h"

class fx_forward : public product
{
public:
	fx_forward(date_t expiry, amount asset_amount, amount base_amount);

	amount pv(currency_code target_ccy) const override;

	double payoff(double St) const override;

	virtual const fx& get_fx() const override { return fx_; }

protected:
	amount asset_amount_;
	const currency& asset_ccy_;
  	const fx& fx_;
};
