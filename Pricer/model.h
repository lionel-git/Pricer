#pragma once
#include <string>
#include <memory>

enum class model_type { BLACK_SCHOLES, NORMAL };
std::string enumToText(model_type);

enum class numerical_method { CLOSED_F, MC, EDP };
std::string enumToText(numerical_method);

class model
{
public:
	static std::unique_ptr<model> get_model(model_type mt);
	void set_numerical_method(numerical_method nm) { numerical_method_ = nm; }

private:
	numerical_method numerical_method_{ numerical_method::CLOSED_F };
};
