#pragma once
#include "model.h"

class normal : public model
{
public:
	normal(const product& product, const numerical_parameters& np);
	virtual model_type get_model_type() const { return model_type::NORMAL; }

	virtual double evaluate_closed_f() const override;
	virtual double evaluate_edp() const override;

	virtual void get_edp_xbounds(double& x_min, double& x_max) const override;

private:
	virtual double get_dS_mc(double normal_value, double S, double dt) const override;

	double vol_n_;
};
