#include "unknown.h"
#include "known.h"
#include <stdexcept>
#include <format>

bool unknown::registering_open_ = false;
int unknown::max_variables_ = 0;
std::vector<std::string> unknown::names_;

void 
unknown::start_registering(int max_variables)
{
	registering_open_ = true;
	max_variables_ = max_variables;
	names_.clear();
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
	auto it = std::find(names_.begin(), names_.end(), name);
	if (it != names_.end())
		throw std::runtime_error(std::format("duplicate unknown: '{}'", name));

	size_t cur_variable_idx = names_.size();
	if (cur_variable_idx >= max_variables_)
		throw std::runtime_error("max_variables count exceeded");
	coeffs_.resize(max_variables_);
	for (size_t i = 0; i < max_variables_; ++i)
		coeffs_[i] = i == cur_variable_idx ? known::One : known::Zero;
	names_.push_back(name);
}

unknown::unknown(const unknown& src, const known& rhs, Operation operation): element("toto")
{
	coeffs_.resize(src.coeffs_.size());
	for (int i = 0; i < coeffs_.size(); ++i)
	{
		switch (operation)
		{
		case Operation::Mult:
			coeffs_[i] = src.coeffs_[i] * rhs;
			break;
		case Operation::Add:
			coeffs_[i] = src.coeffs_[i] + rhs;
			break;
		case Operation::Sub:
			coeffs_[i] = src.coeffs_[i] - rhs;
			break;
		default:
			throw std::runtime_error("Unhandeld case");
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const unknown& u)
{
	int nb_var = std::min(u.coeffs_.size(), u.names_.size());
	for (int i = 0; i < nb_var; ++i)
	{
		os << u.coeffs_[i] << " * " << u.names_[i];
		if (i < nb_var - 1)
			os << std::endl;
		
	}
	return os;
}

unknown
unknown::operator*(const known& rhs) const
{
	return unknown(*this, rhs, Operation::Mult);
}

unknown
unknown::operator+(const known& rhs) const
{
	return unknown(*this, rhs, Operation::Add);
}

unknown
unknown::operator-(const known& rhs) const
{
	return unknown(*this, rhs, Operation::Sub);
}
