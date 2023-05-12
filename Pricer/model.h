#pragma once
#include "defines.h"
#include "product.h"
#include <string>
#include <memory>
#include <vector>

enum class model_type { BLACK_SCHOLES, NORMAL };
std::string enumToText(model_type);

enum class numerical_method { CLOSED_F, MC, EDP };
std::string enumToText(numerical_method);

class model
{
public:
	virtual ~model() = default;

	static std::unique_ptr<model> get_model(model_type mt, const product& product);
	void set_numerical_method(numerical_method nm) { numerical_method_ = nm; }

	virtual model_type get_model_type() const = 0;

	void set_time_parameters(int time_points) { time_points_ = time_points; }
	void set_edp_parameters(double x_min, double x_max, int x_points) { x_min_ = x_min; x_max_ = x_max; x_points_ = x_points; }
	void set_mc_parameters(int simuls) { simuls_ = simuls; }

	void initialize();

private:
	void initialize_common();
	void initialize_edp();
	void initialize_mc();

protected:
	model(const product& product);
	numerical_method numerical_method_{ numerical_method::CLOSED_F };

	// common
	const product& product_;
	int time_points_;
	std::vector<double> t_;

	// edp
	double x_min_;
	double x_max_;
	int x_points_;
	std::vector<double> x_;

	// mc
	int simuls_;
};
