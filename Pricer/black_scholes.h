#pragma once
#include "model.h"

class black_scholes : public model
{
public:
	black_scholes() = default;

	virtual model_type get_model_type() const { return model_type::BLACK_SCHOLES; }
};
