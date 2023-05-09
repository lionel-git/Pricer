#include "currency_manager.h"
#include <stdexcept>

const currency_manager& 
currency_manager::instance()
{
	static currency_manager manager;
	return manager;
}

const currency& 
currency_manager::get_currency(currency_code ccy)
{
	const auto& manager = instance();
	auto it = manager.currency_map_.find(ccy);
	if (it == manager.currency_map_.end())
		throw std::runtime_error("cannot find cur: " + enumToText(ccy));
	return it->second;

}

const currency& 
currency_manager::get_currency(amount amt)
{
	return get_currency(amt.currency_);
}

currency_manager::currency_manager()
{
	currency_map_.emplace(currency_code::USD, currency(currency_code::USD));
	currency_map_.emplace(currency_code::JPY, currency(currency_code::JPY));
}

double 
currency_manager::get_fxspot(currency_code asset, currency_code basis)
{
	const auto& ccy_asset = get_currency(asset);
	const auto& ccy_basis = get_currency(basis);
	return ccy_asset.fxspot_ / ccy_basis.fxspot_;
}

double
currency_manager::get_fxspot(amount asset, amount basis)
{
	return  get_fxspot(asset.currency_, basis.currency_);
}

double
currency_manager::get_fxfwd(date_t expiry, currency_code asset, currency_code basis)
{
	const auto& ccy_asset = get_currency(asset);
	const auto& ccy_basis = get_currency(basis);
	return (ccy_asset.fxspot_ / ccy_basis.fxspot_) * (ccy_asset.getDF(expiry) / ccy_basis.getDF(expiry));
}

double
currency_manager::get_fxfwd(date_t expiry, amount asset, amount basis)
{
	return get_fxfwd(expiry, asset.currency_, basis.currency_);
}
