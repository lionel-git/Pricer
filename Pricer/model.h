#pragma once
#include "defines.h"
#include "product.h"
#include "numerical_parameters.h"

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

private:
	void initialize();
	void initialize_common();
	void initialize_edp();
	void initialize_mc();

protected:
	model(const product& product, const numerical_parameters& np);

	const numerical_parameters& numerical_parameters_;

	// common
	const product& product_;
	std::vector<double> t_;

	// edp
	std::vector<double> x_;
};
