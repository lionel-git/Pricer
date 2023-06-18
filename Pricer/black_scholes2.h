#pragma once

#include "black_scholes.h"

class black_scholes2 : public black_scholes
{
public:
    black_scholes2(const product& product, const numerical_parameters& np);
    virtual model_type get_model_type() const override { return model_type::BLACK_SCHOLES2; }

private:
    virtual void get_edp_xbounds(double& x_min, double& x_max) const override;
    virtual void back_propagate_explicit(std::vector<double>& V, double dt, double r, double U_up, double U_down) const override;
    virtual void back_propagate_cranck_nicholson(std::vector<double>& V, double dt, double r, double U_up, double U_down) const override;
    virtual std::set<double> convert_strikes(const std::set<double>& strikes) const override;

    virtual void initialize_terminal_payoff_and_bounds() override;

    virtual double evaluate_mc() const override;
    virtual double get_dS_mc(double normal_value, double S, double dt) const override;
};

