#pragma once
#include <string>
#include <vector>

class element
{
public:
	bool operator==(const element& rhs) { return name_ == rhs.name_; }

protected:
	element(const std::string& name) : name_{ name } {}
	

protected:
	std::string name_;
};
