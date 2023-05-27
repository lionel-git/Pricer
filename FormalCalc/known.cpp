#include "known.h"
#include <format>

known::known(double m) :
	element(std::format("{}", m))
{
}

known 
known::operator*(const known& rhs) const
{
	return known(std::format("{}*{}", name_, rhs.name_));
}

unknown 
known::operator*(const unknown& rhs) const
{
	return rhs * (*this);
}