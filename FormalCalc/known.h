#pragma once
#include "element.h"

class known : public element
{
public:
	known(const std::string& name) : element(name) {}
	known(double m);

	known operator*(const known& rhs);

private:

};

