#pragma once
#include "model.h"

class normal : public model
{
public:
	normal(const product& product, const numerical_parameters& np);

	virtual model_type get_model_type() const { return model_type::NORMAL; }
};
