#include "known.h"
#include <format>

const known known::Zero("0");
const known known::One("1");


known::known(double m) :
	element(std::format("{}", m))
{
}

known 
known::operator*(const known& rhs) const
{
	if (*this == Zero || rhs==Zero)
		return Zero;
	if (*this == One)
		return rhs;
	if (rhs == One)
		return *this;
	return known(std::format("{}*{}", name_, rhs.name_)); // parentheses?
}

unknown 
known::operator*(const unknown& rhs) const
{
	return rhs * (*this);
}

std::ostream& 
operator<<(std::ostream& os, const known& k)
{
	os << k.name_;
	return os;
}
