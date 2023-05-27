#pragma once
#include "element.h"
#include <set>
#include <ostream>

class known;

class unknown : public element
{
public:
	unknown(const std::string& name);

	static void start_registering(int max_variables);
	static void end_registering();

	unknown operator*(const known& rhs) const;

	friend std::ostream& operator<<(std::ostream& os, const unknown& u);

private:
	void init_coeffs();
	unknown(const unknown& src, const known& rhs);

private:
	static bool registering_open_;
	static int cur_variable_idx_;
	static int max_variables_;
	static std::set<std::string> names_;
	std::vector<known> coeffs_;
};
