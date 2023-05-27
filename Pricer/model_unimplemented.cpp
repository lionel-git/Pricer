#include "model.h"

double
model::evaluate_closed_f() const
{
	THROW(get_error_string("Closed Formulae not implemented"));
}

void
model::get_edp_xbounds(double& /*x_min*/, double& /*x_max*/) const
{
	THROW(get_error_string("Edp Formulae not implemented"));
}

void
model::back_propagate_explicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/, double /*V_up*/, double /*V_down*/) const
{
	THROW(get_error_string("Edp Formulae not implemented"));
}

void
model::back_propagate_implicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/, double /*V_up*/, double /*V_down*/) const
{
	THROW(get_error_string("Edp Formulae not implemented"));
}

void
model::back_propagate_cranck_nicholson(std::vector<double>& /*V*/, double /*dt*/, double /*r*/, double /*V_up*/, double /*V_down*/) const
{
	THROW(get_error_string("Edp Formulae not implemented"));
}

double 
model::get_dS_mc(double /*normal_value*/, double /*S*/, double /*dt*/) const 
{ 
	THROW(get_error_string("MC Formulae not implemented")); 
}