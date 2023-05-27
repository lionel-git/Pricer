#include "unknown.h"
#include "known.h"
#include <stdexcept>
#include <format>

bool unknown::registering_open_ = false;
int unknown::cur_variable_idx_ = -10;
int unknown::max_variables_ = -10;
std::set<std::string> unknown::names_;

void 
unknown::start_registering(int max_variables)
{
	registering_open_ = true;
	cur_variable_idx_ = 0;
	max_variables_ = max_variables;
	names_.clear();
}

void 
unknown::end_registering()
{
	registering_open_ = false;
}

void
unknown::init_coeffs()
{
	if (cur_variable_idx_ >= max_variables_)
		throw std::runtime_error("max_variables count exceeded");
	coeffs_.resize(max_variables_);
	for (size_t i = 0; i < max_variables_; ++i)
		coeffs_[i] = i == cur_variable_idx_ ? known::One : known::Zero;
	++cur_variable_idx_;
}

unknown::unknown(const std::string& name): element(name)
{
	if (!registering_open_)
		throw std::runtime_error(std::format("registering unknown '{}', while registering closed", name));
	if (names_.contains(name))
		throw std::runtime_error(std::format("duplicate unknown: '{}'", name));
	names_.insert(name);
	init_coeffs();
}

unknown::unknown(const unknown& src, const known& rhs): element("toto")
{
	coeffs_.resize(src.coeffs_.size());
	for (int i = 0; i < coeffs_.size(); ++i)
		coeffs_[i] = src.coeffs_[i] * rhs;
}

unknown 
unknown::operator*(const known& rhs) const
{
	return unknown(*this, rhs);
}

std::ostream& operator<<(std::ostream& os, const unknown& u)
{
	for (int i = 0; i < u.coeffs_.size(); ++i)
	{
		os << u.coeffs_[i];
		if (i < u.coeffs_.size() - 1)
			os << std::endl;
	}
	return os;
}
