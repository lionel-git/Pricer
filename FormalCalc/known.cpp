#include "known.h"
#include <format>

const known known::Zero("0");
const known known::One("1");


known::known(double m) :
	element(std::format("{}", m))
{
}

// ===  Forward to unknown operator ===
unknown
known::operator*(const unknown& rhs) const
{
	return rhs * (*this);
}

unknown
known::operator+(const unknown& rhs) const
{
	return rhs + (*this);
}

unknown
known::operator-(const unknown& rhs) const
{
	return rhs - (*this);
}
// ===  End Forward  ===

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

known
known::operator+(const known& rhs) const
{
	if (*this == Zero)
		return rhs;
	if (rhs == Zero)
		return *this;
	return known(std::format("{}+{}", name_, rhs.name_)); // parentheses?
}

known
known::operator-(const known& rhs) const
{
	if (*this == Zero)
		return rhs;
	if (rhs == Zero)
		return *this;
	return known(std::format("{}-{}", name_, rhs.name_)); // parentheses?
}

std::ostream& 
operator<<(std::ostream& os, const known& k)
{
	os << k.name_;
	return os;
}
