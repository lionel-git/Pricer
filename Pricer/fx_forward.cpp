#include "fx_forward.h"
#include "currency_manager.h"
#include "fx_manager.h"
#include "utils.h"

fx_forward::fx_forward(date_t expiry, amount asset_amount, amount base_amount) :
	product(expiry, base_amount),
	asset_amount_(asset_amount), asset_ccy_(currency_manager::get_currency(asset_amount)),
	fx_(fx_manager::get_fx(asset_amount.currency_, base_amount.currency_))
{
	check_opposite_signs(asset_amount.notional_, base_amount.notional_);
}

amount
fx_forward::pv(currency_code target_ccy) const
{
	return (asset_amount_ * asset_ccy_.getDF(expiry_)).countervalue(target_ccy) +
		   (base_amount_  * base_ccy_.getDF(expiry_)).countervalue(target_ccy);
}
