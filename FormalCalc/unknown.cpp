#include "unknown.h"
#include <stdexcept>
#include <format>

bool unknown::registering_open_ = false;
std::vector<element> unknown::list_;

void 
unknown::start_registering()
{
	list_.clear();
	registering_open_ = true;
}

void 
unknown::end_registering()
{
	registering_open_ = false;
}

unknown::unknown(const std::string& name): element(name)
{
	if (!registering_open_)
		throw std::runtime_error(std::format("registering unknown '{}', while registering closed", name));
	auto it = std::find(list_.begin(), list_.end(), *this);
	if (it != list_.end())
		throw std::runtime_error(std::format("duplicate unknown: '{}'", name));
	list_.push_back(*this);
}
