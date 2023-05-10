#pragma once
#include "fx.h"
#include "amount.h"

#include <map>

class fx_manager
{
public:
	static const fx& get_fx(fx_key fx_key);
	static const fx& get_fx(currency_code asset, currency_code basis);

private:
	fx_manager();
	fx_manager(const fx_manager&) = delete;
	fx_manager(const fx_manager&&) = delete;
	fx_manager& operator=(const fx_manager&) = delete;
	static const fx_manager& instance();
	void add_fx(currency_code asset, currency_code basis, double fxspot, double fxvol);

	std::map<fx_key, fx> fx_map_;
};

