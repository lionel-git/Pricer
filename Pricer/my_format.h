#pragma once

#if __has_include(<format>)
#include <format>
namespace myfmt = std;
#else
#include <fmt/format.h>
namespace myfmt = fmt;
#endif
