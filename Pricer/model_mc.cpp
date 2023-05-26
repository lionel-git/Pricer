// The mc specific methods for class model
#include "model.h"
#include "numerical_parameters_mc.h"
#include "brownian.h"

void
model::initialize_mc()
{
	initialize_common();
}

double
model::evaluate_mc() const
{
	const numerical_parameters_mc& params = dynamic_cast<const numerical_parameters_mc&>(numerical_parameters_);
	double sum_payoffs = 0.0;
	brownian b;
	for (int simul = 0; simul < params.simuls_; ++simul)
	{
		double St = product_.get_fx().get_spot();
		for (size_t j = 1; j < t_.size(); ++j)
		{
			// on diffuse le spot entre t[j-1] et t[j]
			double dt = t_[j] - t_[j - 1];
			double dS_deter = St * r_[j] * dt;
			double dS_stochastic = get_dS_mc(b.normal_value(), St, dt);
			St += dS_deter + dS_stochastic;
		}
		sum_payoffs += product_.payoff(St);
	}
	return (sum_payoffs / params.simuls_) * basis_.get_df(product_.get_expiry());
}

