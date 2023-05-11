#pragma once
#include <stdexcept>
#include <string>

class pricer_exception : public std::runtime_error
{
public:
	pricer_exception(const std::string& msg);
	pricer_exception(const std::string& msg, const std::string& func_name, const std::string& filename, int line);
};

#define THROW(MSG) { throw pricer_exception((MSG), __FUNCTION__, __FILE__, __LINE__); }
