#include "numerical_parameters.h"
#include "utils.h"

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
