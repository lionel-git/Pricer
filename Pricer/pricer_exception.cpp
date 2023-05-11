#include "pricer_exception.h"
#include <sstream>

pricer_exception::pricer_exception(const std::string& msg):
	runtime_error(msg)
{
}

static std::string
get_msg(const std::string& msg, const std::string& func_name, const std::string& filename, int line)
{
	std::ostringstream os;
	os << msg << " (" << func_name << ") " << filename << ":" << line;
	return os.str();
}

pricer_exception::pricer_exception(const std::string & msg, const std::string & func_name, const std::string & filename, int line)
	: runtime_error(get_msg(msg, func_name, filename, line))
{
}
