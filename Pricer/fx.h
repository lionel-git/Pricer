#pragma once
#include "currency.h"

// Order is asset, basis
using fx_key = std::pair<currency_code, currency_code>;
std::string to_string(fx_key fx_key);

class fx
{
public:
	fx(fx_key fx_key, double fxspot, double fxvol);

	double getSpot() const { return fxspot_; }
	double getFwd(date_t expiry) const;
	double getVol() const { return fxvol_; }

private:
	fx_key fx_key_;
	const currency& asset_;
	const currency& basis_;
	double fxspot_;
	double fxvol_;
};
