#include "fx_forward.h"
#include "currency_manager.h"
#include "fx_manager.h"
#include "utils.h"

fx_forward::fx_forward(date_t expiry, amount asset_amount, amount base_amount) :
	product(expiry, base_amount),
	asset_amount_(asset_amount), asset_ccy_(currency_manager::get_currency(asset_amount)),
	fx_(fx_manager::get_fx(asset_amount.currency_, base_amount.currency_))
{
	// On paie a  maturite : Na (AAA) + Nb (BBB) (Na & Nb should have oposite signs) 
	// spot S = AAABBB
	// en base currency, pv_forward = Na.S + Nb = Na. (S + Nb/Na)
	// pv_forward = Na. (S - K) avec K = -Nb/Na
	// pv_forward = |Na| . (sign.(S - K)) avec sign = sign(Na), rem : |Na| = sign * Na
	strike_ = -base_amount.notional_ / asset_amount_.notional_; // should be positive
	sign_ = check_sign(asset_amount.notional_); // Eqv to call/put
}

amount
fx_forward::pv(currency_code target_ccy) const
{
	double spot_payoff = payoff(fx_.get_fwd(expiry_));
	auto amount_base_ccy = asset_amount_.strike_countervalue(base_currency(), spot_payoff) * base_ccy_.get_df(expiry_);
	return amount_base_ccy.spot_countervalue(target_ccy);
}

double
fx_forward::payoff(double St) const
{
	return St - strike_;
}
