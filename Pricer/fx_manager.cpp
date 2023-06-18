#include "fx_manager.h"
#include "pricer_exception.h"

#include <format>
#include <iostream>

const fx_manager&
fx_manager::instance(int setup_idx)
{
    static const fx_manager manager(setup_idx);
    if (setup_idx != 0 && setup_idx != manager.setup_idx_)
        THROW(std::format("Change of setup: {} => {}", manager.setup_idx_, setup_idx));
    return manager;
}

void
fx_manager::add_fx(currency_code asset, currency_code basis, double fxspot, double fxvol)
{
    auto fx_key = std::make_pair(asset, basis);
    fx_map_.emplace(fx_key, fx(fx_key, fxspot, fxvol));
}

fx_manager::fx_manager(int setup_idx)
{
    std::cout << "Init fx_manager with setup: " << setup_idx << std::endl;
    switch (setup_idx)
    {
    case 0:
        add_fx(currency_code::USD, currency_code::JPY, 135.0, 10.0 / 100.0);
        add_fx(currency_code::USD, currency_code::EUR, 0.91, 10.0 / 100.0);
        add_fx(currency_code::USD, currency_code::HKD, 7.84, 10.0 / 100.0);
        break;
    case 1:
        add_fx(currency_code::USD, currency_code::JPY, 135.0, 10.0 / 100.0);
        add_fx(currency_code::USD, currency_code::EUR, 0.91, 10.0 / 100.0);
        add_fx(currency_code::USD, currency_code::HKD, 7.84, 10.0 / 100.0);
        break;
    default:
        THROW(std::format("Invalid setup_idx: {}", setup_idx));
    }
    setup_idx_ = setup_idx;
}

const fx&
fx_manager::get_fx(fx_key fx_key)
{
    const auto& manager = instance(0);
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
