#pragma once
#include "amount.h"
#include "fx.h"
#include "pricer_exception.h"
#include <set>

enum class pde_underlying { FXSPOT, VOLATILITY };

class product
{
public:
	product(date_t expiry, amount base_amount);
	virtual ~product() = default;

	currency_code base_currency() const { return base_amount_.currency_; }
	virtual amount pv(currency_code target_ccy) const;

	// pv of payoff at maturity in base currency, given the spot at maturity
	// without notional
	virtual double payoff(double St) const;

	date_t get_expiry() const { return expiry_; }
	double get_strike() const { return strike_; }
	double get_sign() const { return sign_;  }

	virtual const fx& get_fx() const { THROW("Not an fx product"); }

	virtual std::set<double> get_critical_times() const { return {}; }
	virtual std::set<double> get_critical_strikes(pde_underlying /*underlying*/) const { return {}; }

protected:
	date_t expiry_;
	amount base_amount_;
	const currency& base_ccy_;
	double strike_;
	double sign_;
};
