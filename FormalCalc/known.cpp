#include "known.h"
#include <format>

known::known(double m) :
	element(std::format("{}", m))
{
}

known 
known::operator*(const known& rhs)
{
	return known(std::format("{}*{}", name_, rhs.name_));
}
