#pragma once
#include "model.h"

class black_scholes : public model
{
public:
	black_scholes(const product& product, const numerical_parameters& np);
	virtual model_type get_model_type() const { return model_type::BLACK_SCHOLES; }

	virtual double evaluate_closed_f() const override;
	virtual double evaluate_edp() const override;
	virtual double evaluate_mc() const override;
};
