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
	sign_ = check_sign(asset_amount.notional_); // Eqv to call/put
}

amount 
fx_option::pv(currency_code target_ccy) const
{
	// pv_option = |Na| . E[(sign.(S - K))+]
	// instric value for now
	double spot_payoff = payoff(fx_.get_fwd(expiry_));
	auto amount_base_ccy = asset_amount_.strike_countervalue(base_currency(), sign_ * spot_payoff) * base_ccy_.get_df(expiry_);
	return amount_base_ccy.spot_countervalue(target_ccy);
}

double
fx_option::payoff(double St) const
{
	return relu(sign_ * (St - strike_));
}

