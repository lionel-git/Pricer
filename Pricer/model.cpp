#include "model.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "pricer_exception.h"
#include "black_scholes.h"
#include "normal.h"

std::string 
enumToText(model_type mt)
{
	switch (mt)
	{
	case model_type::BLACK_SCHOLES:
		return "BS";
	case model_type::NORMAL:
		return "NORMAL";
	default:
		THROW("Unkown model type?");
	}

}

model::model(const product& product, const numerical_parameters& np) : 
	product_(product), numerical_parameters_(np), asset_(product.get_fx().get_asset()), basis_(product.get_fx().get_basis())
{
	initialize();
}

static
void initialize_points(std::vector<double>& v, double min, double max, int nb_points)
{
	if (nb_points < 2)
		THROW("Not enough points");
	v.resize(nb_points);
	double dx = (max - min) / (v.size() - 1);
	for (int i = 0; i < v.size(); ++i)
		v[i] = min + i * dx; // x0=min, x_end=max
}

void
model::initialize_common()
{
	initialize_points(t_, 0.0, product_.get_expiry(), numerical_parameters_.time_points_);
}

void 
model::initialize_edp()
{
	initialize_common();
	const numerical_parameters_edp& params = dynamic_cast<const numerical_parameters_edp&>(numerical_parameters_);
	initialize_points(x_, params.x_min_, params.x_max_, params.x_points_);
}

void 
model::initialize_mc()
{
	initialize_common();
}

void 
model::initialize()
{
	switch (get_numerical_method())
	{
	case numerical_method::CLOSED_F:
		return;
	case numerical_method::EDP:
		return initialize_edp();
	case numerical_method::MC:
		return initialize_mc();
	default:
		THROW("Unkown numerical method?");
	}
}

double
model::evaluate() const
{
	switch (get_numerical_method())
	{
	case numerical_method::CLOSED_F:
		return evaluate_closed_f();
	case numerical_method::EDP:
		return evaluate_edp();
	case numerical_method::MC:
		return evaluate_mc();
	default:
		THROW("Unkown numerical method?");
	}
}