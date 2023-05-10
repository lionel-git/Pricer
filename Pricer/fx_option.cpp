#include "fx_option.h"
#include "utils.h"

fx_option::fx_option(date_t expiry, amount asset_amount, amount base_amount) :
	fx_forward(expiry, asset_amount, base_amount)
{
	// on a l'option d'exercer le forward: Na (AAA) + Nb (BBB) (Na & Nb have oposite signs) 
	// on exerce si la valeur est positive
	// spot S = AAABBB
	// en base currency, pv_forward = Na.S + Nb = Na. (S + Nb/Na)
	// pv_forward = Na. (S - K) avec K = -Nb/Na
	// pv_forward = |Na| . (sign.(S - K)) avec sign = sign(Na), rem : |Na| = sign * Na
	// pv_option = |Na| . E[(sign.(S - K))+]
	strike_ = -base_amount.notional_ / asset_amount_.notional_; // should be positive
	sign_ = check_sign(asset_amount.notional_); // Eqv to call/put
}

amount 
fx_option::pv(currency_code target_ccy) const
{
	// pv_option = |Na| . E[(sign.(S - K))+]
	// instric value for the moment
	double spot_payoff = relu(sign_ * (fx_.getFwd(expiry_) - strike_));
	auto amount_base_ccy = asset_amount_.strike_countervalue(base_currency(), sign_ * spot_payoff) * base_ccy_.getDF(expiry_);
	return amount_base_ccy.countervalue(target_ccy);
}
