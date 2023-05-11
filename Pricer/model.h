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
	virtual ~model() = default;

	static std::unique_ptr<model> get_model(model_type mt);
	void set_numerical_method(numerical_method nm) { numerical_method_ = nm; }

	virtual model_type get_model_type() const = 0;

	void set_edp_parameters(int time_steps, int x_steps) { time_steps_ = time_steps; x_steps_ = x_steps; }
	void set_mc_parameters(int time_steps, int simuls) { time_steps_ = time_steps; simuls_ = simuls; }

	void initialize();

private:
	numerical_method numerical_method_{ numerical_method::CLOSED_F };
	// common
	int time_steps_;
	// edp
	int x_steps_;
	// mc
	int simuls_;
};
