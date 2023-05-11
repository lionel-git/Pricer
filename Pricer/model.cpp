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
model::get_model(model_type mt)
{
	switch (mt)
	{
	case model_type::BLACK_SCHOLES:
		return std::make_unique<black_scholes>();
	case model_type::NORMAL:
		return std::make_unique<normal>();
	default:
		THROW("Unkown numerical method?");
	}
}

void 
model::initialize()
{


}


