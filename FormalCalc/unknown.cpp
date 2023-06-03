#include "unknown.h"
#include "known.h"
#include <stdexcept>
#include <format>
#include <algorithm>
#include <sstream>

bool unknown::registering_open_ = false;
size_t unknown::max_variables_ = 0;
std::vector<std::string> unknown::names_;

void
unknown::start_registering(int max_variables)
{
    registering_open_ = true;
    max_variables_ = 1 + max_variables; // 1 constant + variables
    names_.clear();
    names_.push_back("_cst_");
}

void
unknown::end_registering()
{
    registering_open_ = false;
}

unknown::unknown(const std::string& name) : element(name)
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

unknown::unknown(const unknown& src, const known& rhs, Operation operation) : element("toto")
{
    coeffs_.resize(src.coeffs_.size());
    for (size_t i = 0; i < coeffs_.size(); ++i)
    {
        switch (operation)
        {
        case Operation::Mult:
            coeffs_[i] = rhs * src.coeffs_[i];
            break;
        case Operation::Add:
            if (i == 0)
                coeffs_[i] = src.coeffs_[i] + rhs;
            else
                coeffs_[i] = src.coeffs_[i];
            break;
        case Operation::Sub:
            if (i == 0)
                coeffs_[i] = src.coeffs_[i] - rhs;
            else
                coeffs_[i] = src.coeffs_[i];
            break;
        default:
            throw std::runtime_error("Unhandeld case");
            break;
        }
    }
}

unknown::unknown(const unknown& src, const unknown& rhs, Operation operation) : element("titi")
{
    if (src.coeffs_.size() != rhs.coeffs_.size())
        throw std::runtime_error("Sizes do not match");
    coeffs_.resize(src.coeffs_.size());
    for (size_t i = 0; i < coeffs_.size(); ++i)
    {
        switch (operation)
        {
        case Operation::Mult:
            throw std::runtime_error("Invalid, mult between 2 unknown!");
            break;
        case Operation::Add:
            coeffs_[i] = src.coeffs_[i] + rhs.coeffs_[i];
            break;
        case Operation::Sub:
            coeffs_[i] = src.coeffs_[i] - rhs.coeffs_[i];
            break;
        default:
            throw std::runtime_error("Unhandeld case");
            break;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const unknown& u)
{
    size_t nb_var = std::min(u.coeffs_.size(), u.names_.size());
    for (size_t i = 0; i < nb_var; ++i)
    {
        if (i != 0)
            os << "mult of ";
        else
            os << "        ";
        os << u.names_[i] << "\t: " << u.coeffs_[i];
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


unknown
unknown::operator+(const unknown& rhs) const
{
    return unknown(*this, rhs, Operation::Add);
}

unknown
unknown::operator-(const unknown& rhs) const
{
    return unknown(*this, rhs, Operation::Sub);
}

// operator* not handled (non linear equations)

// Transform an unknown into a known element
unknown&
unknown::operator=(const known& rhs)
{
    name_ = rhs.name_;
    coeffs_[0] = rhs;
    for (size_t i = 1; i < coeffs_.size(); ++i)
        coeffs_[i] = known::Zero;
    return *this;
}

std::string
unknown::get_code(const std::vector<std::string>& start_code, const std::vector<std::string>& end_code)
{
    std::ostringstream os;
    for (const auto& line : start_code)
        os << line << std::endl;
    // matrix row: a[i], b[i], c[i]
    // First row: b[0], c[0]
    // Last row : a[n-1], b[n-1]
    // vecteur target : t[i]
    os << "t[i] = - (" << coeffs_[0] << ");" << std::endl;
    os << "a[i] = " << coeffs_[1] << ";" << std::endl;
    os << "b[i] = " << coeffs_[2] << ";" << std::endl;
    os << "c[i] = " << coeffs_[3] << ";" << std::endl;
    for (size_t i = 0; i < end_code.size(); ++i)
    {
        os << end_code[i];
        if (i < end_code.size() - 1)
            os << std::endl;
    }
    return os.str();
}

