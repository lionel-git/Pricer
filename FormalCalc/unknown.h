#pragma once
#include "element.h"

class known;

class unknown : public element
{
public:
	unknown(const std::string& name);

	static void start_registering();
	static void end_registering();

	void init_coeffs(size_t idx);

	unknown operator*(const known& rhs) const;

private:
	unknown(const unknown& src, const known& rhs);

	static std::vector<unknown> base_elements_;
	static bool registering_open_;
	std::vector<known> coeffs_;
};
