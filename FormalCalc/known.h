#pragma once
#include "element.h"
#include "unknown.h"

class known : public element
{
public:
	known() :element("???") {};
	known(const std::string& name) : element(name) {}
	known(double m);

	known operator*(const known& rhs) const;
	unknown operator*(const unknown& rhs) const;

private:

};

