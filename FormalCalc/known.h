#pragma once
#include "element.h"
#include "unknown.h"

class known : public element
{
public:
	known() :element("???") {};
	known(const std::string& name) : element(name) {}
	known(double m);

	bool operator==(const known& rhs) const { return name_ == rhs.name_; }

	known operator*(const known& rhs) const;
	unknown operator*(const unknown& rhs) const;

	known operator+(const known& rhs) const;
	unknown operator+(const unknown& rhs) const;

	known operator-(const known& rhs) const;
	unknown operator-(const unknown& rhs) const;

	known operator-() const;

	friend class unknown;
	friend std::ostream& operator<<(std::ostream& os, const known& k);

	static const known Zero;
	static const known One;

private:

};

