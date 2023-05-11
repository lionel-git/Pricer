#include "fx_manager.h"
#include "pricer_exception.h"

const fx_manager&
fx_manager::instance()
{
	static const fx_manager manager;
	return manager;
}

void
fx_manager::add_fx(currency_code asset, currency_code basis, double fxspot, double fxvol)
{
	auto fx_key = std::make_pair(asset, basis);
	fx_map_.emplace(fx_key, fx(fx_key, fxspot, fxvol));
}

fx_manager::fx_manager()
{
	add_fx(currency_code::USD, currency_code::JPY, 135.0, 50.0 / 100.0);
	add_fx(currency_code::USD, currency_code::EUR, 0.91, 50.0 / 100.0);
	add_fx(currency_code::USD, currency_code::HKD, 7.84, 50.0 / 100.0);
}

const fx&
fx_manager::get_fx(fx_key fx_key)
{
	const auto& manager = instance();
	auto it = manager.fx_map_.find(fx_key);
	if (it == manager.fx_map_.end())
		THROW("cannot find fxspot: " + to_string(fx_key));
	return it->second;
}

const fx& 
fx_manager::get_fx(currency_code asset, currency_code basis)
{
	return get_fx(std::make_pair(asset, basis));
}
