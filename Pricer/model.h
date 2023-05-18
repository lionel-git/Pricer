#pragma once
#include "defines.h"
#include "product.h"
#include "numerical_parameters.h"
#include "pricer_exception.h"
#include "edp_coeffs.h"

#include <string>
#include <memory>
#include <vector>

enum class model_type { INVALID, BLACK_SCHOLES, NORMAL };
std::string enumToText(model_type);

class model
{
public:
	virtual ~model() = default;
	virtual model_type get_model_type() const { return model_type::INVALID; }
	numerical_method get_numerical_method() const { return numerical_parameters_.get_numerical_method(); }

	double evaluate() const;

private:
	void initialize_common();
	void initialize_edp();
	void initialize_edp_coeffs();
	void initialize_mc();

protected:
	model(const product& product, const numerical_parameters& np);
	void initialize();

	std::string get_error_string(const std::string& msg) const { return "model "+ enumToText(get_model_type()) + ": " + msg; }

	// === Closed formula ====
	virtual double evaluate_closed_f() const { THROW(get_error_string("Closed Formulae not implemented")); }
	// =======================

	// === EDP ====
	double evaluate_edp() const;
	virtual void get_edp_xbounds(double& /*x_min*/, double& /*x_max*/) const { THROW(get_error_string("Edp Formulae not implemented")); }
	void initialize_terminal_payoff(std::vector<double>& V) const;
	virtual void back_propagate_explicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/) const { THROW(get_error_string("Edp Formulae not implemented")); }
	virtual void back_propagate_implicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/) const { THROW(get_error_string("Edp Formulae not implemented")); }
	virtual void back_propagate_cranck_nicholson(std::vector<double>& /*V*/, double /*dt*/, double /*r*/) const { THROW(get_error_string("Edp Formulae not implemented")); }
	// =======================
	 
	// === MC ===
	double evaluate_mc() const;
	// normal_value is random value from distrib with mean=0, std_dev=1
	virtual double get_dS_mc(double /*normal_value*/, double /*S*/, double /*dt*/) const { THROW(get_error_string("MC Formulae not implemented")); }
	// =======================

	const numerical_parameters& numerical_parameters_;

	// market data
	const currency& asset_;
	const currency& basis_;

	// common
	const product& product_;
	std::vector<double> t_;
	std::vector<double> r_; // ratio of df(ti) or ln() ?

	// edp
	std::vector<double> x_;
	std::vector<edp_coeffs> edp_coeffs_;
};
