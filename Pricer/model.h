#pragma once
#include "defines.h"
#include "product.h"
#include "numerical_parameters.h"
#include "pricer_exception.h"

#include <string>
#include <memory>
#include <vector>

enum class model_type { BLACK_SCHOLES, NORMAL };
std::string enumToText(model_type);

class model
{
public:
	virtual ~model() = default;
	virtual model_type get_model_type() const = 0;
	numerical_method get_numerical_method() const { return numerical_parameters_.get_numerical_method(); }

	double evaluate() const;

private:
	void initialize();
	void initialize_common();
	void initialize_edp();
	void initialize_mc();

protected:
	model(const product& product, const numerical_parameters& np);

	std::string get_error_string(const std::string& msg) const { return "model "+ enumToText(get_model_type()) + ": " + msg; }

	virtual double evaluate_closed_f() const { THROW(get_error_string("Closed Formulae not implemented")); }
	virtual double evaluate_edp() const { THROW(get_error_string("Edp Formulae not implemented")); }
	virtual double evaluate_mc() const;

	// normal_value is random value from distrib with mean=0, std_dev=1
	virtual double get_dS_mc(double /*normal_value*/, double /*S*/, double /*dt*/) const { THROW(get_error_string("MC Formulae not implemented")); }

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
};
