#include "fx.h"
#include "currency_manager.h"

std::string to_string(fx_key fx_key)
{
	return enumToText(fx_key.first) + enumToText(fx_key.second);
}

fx::fx(fx_key fx_key, double fxspot, double fxvol):
	fx_key_(fx_key), 
	asset_(currency_manager::get_currency(fx_key.first)), basis_(currency_manager::get_currency(fx_key.second)),
	fxspot_(fxspot), fxvol_(fxvol)
{
}

double 
fx::get_fwd(date_t expiry) const
{
	return get_spot() * asset_.get_df(expiry) / basis_.get_df(expiry);
}

double 
fx::get_normal_vol() const
{
	// We have dS = SigmaBS.S.dW
	// We have dS = SigmaN.dW
	// SigmaN = SigmaBS.S
	return fxvol_ * fxspot_;
}
