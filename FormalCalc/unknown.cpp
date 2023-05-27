#include "unknown.h"
#include "known.h"
#include <stdexcept>
#include <format>

bool unknown::registering_open_ = false;
std::vector<unknown> unknown::base_elements_;

void 
unknown::start_registering()
{
	base_elements_.clear();
	registering_open_ = true;
}

void 
unknown::end_registering()
{
	registering_open_ = false;
	for (size_t i = 0; i < base_elements_.size(); ++i)
		base_elements_[i].init_coeffs(i);
}

void
unknown::init_coeffs(size_t idx)
{
	if (idx >= base_elements_.size())
		throw std::runtime_error("idx out of range");
	coeffs_.resize(base_elements_.size());
	for (size_t i = 0; i < base_elements_.size(); ++i)
		coeffs_[i] = i == idx ? known("1") : known("0");
}

unknown::unknown(const std::string& name): element(name)
{
	if (!registering_open_)
		throw std::runtime_error(std::format("registering unknown '{}', while registering closed", name));
	auto it = std::find(base_elements_.begin(), base_elements_.end(), *this);
	if (it != base_elements_.end())
		throw std::runtime_error(std::format("duplicate unknown: '{}'", name));
	base_elements_.push_back(*this);
}

unknown::unknown(const unknown& src, const known& rhs): element("toto")
{
	coeffs_.resize(src.coeffs_.size());
	for (int i = 0; i < coeffs_.size(); ++i)
		coeffs_[i] = coeffs_[i] * rhs;
}

unknown 
unknown::operator*(const known& rhs) const
{
	return unknown(*this, rhs);
}
