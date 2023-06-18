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
    return unknown(*this) - rhs;
}
// ===  End Forward  ===

known
known::operator*(const known& rhs) const
{
    if (*this == Zero || rhs == Zero)
        return Zero;
    if (*this == One)
        return rhs;
    if (rhs == One)
        return *this;
    return known(std::format("{}*{}", name_, rhs.name_)); // '*' est prioritaire sur + / -
}

known
known::operator+(const known& rhs) const
{
    if (*this == Zero)
        return rhs;
    if (rhs == Zero)
        return *this;
    return known(std::format("({}+{})", name_, rhs.name_));
}

known
known::operator-(const known& rhs) const
{
    if (*this == Zero)
        return -rhs; // Warning: 0-x = -x !
    if (rhs == Zero)
        return *this;
    return known(std::format("({}-{})", name_, rhs.name_));
}

known
known::operator-() const
{
    if (*this == Zero)
        return Zero;
    return known(std::format("(-{})", name_));
}

std::ostream&
operator<<(std::ostream& os, const known& k)
{
    std::string name = k.name_;
    // marche pas sur cas : (1-x)*(y+1)
    /*while (name.size() > 0 && name[0] == '(' && name[name.size() - 1] == ')')
        name = name.substr(1, name.size() - 2);	*/
    os << name;
    return os;
}
