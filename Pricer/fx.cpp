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
fx::getFwd(date_t expiry) const
{
	return getSpot() * asset_.getDF(expiry) / basis_.getDF(expiry);
}
