#pragma once
#include "defines.h"
#include "product.h"
#include "numerical_parameters.h"
#include "edp_coeffs.h"

#include <string>
#include <memory>
#include <vector>

enum class model_type { INVALID, BLACK_SCHOLES, BLACK_SCHOLES2, NORMAL };
std::string enumToText(model_type);

class model
{
public:
    virtual ~model() = default;
    virtual model_type get_model_type() const { return model_type::INVALID; }
    numerical_method get_numerical_method() const { return numerical_parameters_.get_numerical_method(); }

    virtual void check_parameters() const = 0;

    double evaluate() const;

    void set_theta(double theta);
    double get_theta() const { return theta_; }

private:
    void initialize_common();
    void initialize_edp();
    void initialize_edp_coeffs();
    void initialize_mc();

    static void initialize_grid_axis(std::vector<double>& v, double min, double max, int nb_points, const std::set<double>& critical_points, double percent_dx);

protected:
    model(const product& product, const numerical_parameters& np);
    void initialize();
    virtual std::set<double> convert_strikes(const std::set<double>& strikes) const { return strikes; }

    std::string get_error_string(const std::string& msg) const { return "model " + enumToText(get_model_type()) + ": " + msg; }


    // === Closed formula ====
    virtual double evaluate_closed_f() const;
    // =======================

    // === EDP ====
    double evaluate_edp() const;
    virtual void get_edp_xbounds(double& x_min, double& x_max) const;
    virtual void initialize_terminal_payoff_and_bounds();
    virtual void back_propagate_explicit(std::vector<double>& V, double dt, double r, double U_up, double U_down) const;
    virtual void back_propagate_cranck_nicholson(std::vector<double>& V, double dt, double r, double U_up, double U_down) const;
    void check_edp_params() const;
    // =======================

    // === MC ===
    virtual double evaluate_mc() const;
    // normal_value is random value from distrib with mean=0, std_dev=1
    virtual double get_dS_mc(double normal_value, double S, double dt) const;
    // =======================

    // market data
    const currency& asset_;
    const currency& basis_;

    // common
    const product& product_;
    const numerical_parameters& numerical_parameters_;
    std::vector<double> t_;
    double eps_percent_dt_;
    // taux instantane
    std::vector<double> r_;
    std::vector<double> r_log_;
    std::vector<double> r_asset_;
    std::vector<double> r_basis_;

    // edp
    std::vector<double> x_;
    std::vector<edp_coeffs> edp_coeffs_;
    double eps_percent_dx_;
    std::vector<double> V_terminal_;   // same size as x_ and V
    std::vector<double> V_bound_up_;   // same size as t_
    std::vector<double> V_bound_down_; // same size as t_
    double theta_{ 0.5 };              // For cranck nicholson
};
