#include "model.h"
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

std::string 
enumToText(numerical_method nm)
{
	switch (nm)
	{
	case numerical_method::CLOSED_F:
		return "CLOSED_F";
	case numerical_method::MC:
		return "MC";
	case numerical_method::EDP:
		return "EDP";
	default:
		THROW("Unkown numerical method?");
	}
}

std::unique_ptr<model> 
model::get_model(model_type mt, const product& product)
{
	switch (mt)
	{
	case model_type::BLACK_SCHOLES:
		return std::make_unique<black_scholes>(product);
	case model_type::NORMAL:
		return std::make_unique<normal>(product);
	default:
		THROW("Unkown numerical method?");
	}
}

model::model(const product& product) : product_(product)
{
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
	initialize_points(t_, 0.0, product_.get_expiry(), time_points_);
}

void 
model::initialize_edp()
{
	initialize_common();
	initialize_points(x_, x_min_, x_max_, x_points_);
}

void 
model::initialize_mc()
{
	initialize_common();
}

void 
model::initialize()
{
	switch (numerical_method_)
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
