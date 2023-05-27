#pragma once
#include "element.h"

class unknown : public element
{
public:
	unknown(const std::string& name);

	static void start_registering();
	static void end_registering();

private:
	static std::vector<element> list_;
	static bool registering_open_;
};
