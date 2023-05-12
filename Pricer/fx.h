#pragma once
#include "currency.h"

// Order is asset, basis
using fx_key = std::pair<currency_code, currency_code>;
std::string to_string(fx_key fx_key);

class fx
{
public:
	fx(fx_key fx_key, double fxspot, double fxvol);

	double get_spot() const { return fxspot_; }
	double get_fwd(date_t expiry) const;
	double get_bs_vol() const { return fxvol_; }
	double get_normal_vol() const;

private:
	fx_key fx_key_;
	const currency& asset_;
	const currency& basis_;
	double fxspot_;
	double fxvol_;
};
