#include "fx_forward.h"
#include "currency_manager.h"

fx_forward::fx_forward(date_t expiry, amount asset_amount, amount base_amount) :
	product(expiry, base_amount), asset_amount_(asset_amount), asset_ccy_(currency_manager::get_currency(asset_amount))
{
}

amount
fx_forward::pv() const
{
	auto pv_base = product::pv();
	auto pv_asset = asset_amount_ * asset_ccy_.getDF(expiry_);
	return pv_base + pv_asset.countervalue(base_amount_.currency_);
}