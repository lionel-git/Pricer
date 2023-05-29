#pragma once
#include "fx.h"
#include "amount.h"

#include <map>

class unit_tests;

class fx_manager
{
public:
	static const fx& get_fx(fx_key fx_key);
	static const fx& get_fx(currency_code asset, currency_code basis);

	friend unit_tests;

private:
	fx_manager(int setup_idx);
	fx_manager(const fx_manager&) = delete;
	fx_manager(const fx_manager&&) = delete;
	fx_manager& operator=(const fx_manager&) = delete;
	static const fx_manager& instance(int setup_idx);
	void add_fx(currency_code asset, currency_code basis, double fxspot, double fxvol);

	std::map<fx_key, fx> fx_map_;

	int setup_idx_{ -1 };
};

