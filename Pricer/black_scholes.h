#pragma once
#include "model.h"

class black_scholes : public model
{
public:
	black_scholes(const product& product, const numerical_parameters& np);
	virtual model_type get_model_type() const override { return model_type::BLACK_SCHOLES; }

	virtual void check_parameters() const override;
	virtual double evaluate_closed_f() const override;

private:
	virtual void get_edp_xbounds(double& x_min, double& x_max) const override;
	virtual void back_propagate_explicit(std::vector<double>& V, double dt, double r) const override;
	virtual void back_propagate_implicit(std::vector<double>& V, double dt, double r) const override;
	virtual void back_propagate_cranck_nicholson(std::vector<double>& V, double dt, double r) const override;

	virtual double get_dS_mc(double normal_value, double S, double dt) const override;

	double vol_bs_;
};
