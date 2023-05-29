#pragma once
#include "element.h"
#include <set>
#include <ostream>

class known;

class unknown : public element
{
public:
	unknown(const std::string& name);

	unknown& operator=(const known& rhs);

	static void start_registering(int max_variables);
	static void end_registering();

	unknown operator*(const known& rhs) const;
	unknown operator+(const known& rhs) const;
	unknown operator-(const known& rhs) const;

	unknown operator+(const unknown& rhs) const;
	unknown operator-(const unknown& rhs) const;

	friend std::ostream& operator<<(std::ostream& os, const unknown& u);

	std::string get_code(const std::vector<std::string>& start_code, const std::vector<std::string>& end_code);

private:
	enum class Operation { Mult, Add, Sub };

	unknown(const unknown& src, const known& rhs, Operation operation);
	unknown(const unknown& src, const unknown& rhs, Operation operation);

private:
	static bool registering_open_;
	static size_t max_variables_;
	static std::vector<std::string> names_;
	std::vector<known> coeffs_; // index 0 is the constant coefficient
};
