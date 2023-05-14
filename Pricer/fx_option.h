#pragma once
#include"fx_forward.h"

class fx_option : public fx_forward
{
public:
	fx_option(date_t expiry, amount asset_amount, amount base_amount);

	amount pv(currency_code target_ccy) const override;
	double payoff(double St) const override;
};

