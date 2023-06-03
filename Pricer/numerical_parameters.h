#pragma once

#include <string>

enum class numerical_method { CLOSED_F, MC, EDP };
std::string enumToText(numerical_method);

class numerical_parameters
{
public:
	numerical_parameters(int time_points) { time_points_ = time_points; }
        virtual ~numerical_parameters() = default;
        virtual numerical_method get_numerical_method() const { return numerical_method::CLOSED_F; }	

public:
	int time_points_;
};
