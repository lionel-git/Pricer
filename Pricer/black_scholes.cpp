#include "black_scholes.h"
#include "fx_manager.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"


black_scholes::black_scholes(const product& product, const numerical_parameters& np) :
	model(product, np), vol_bs_(product_.get_fx().get_bs_vol())
{
}

double
black_scholes::evaluate_closed_f() const
{


	return 0.0;
}

double
black_scholes::evaluate_edp() const
{
	return 0.0;
}

// Remonter les boucles dans model
// Faire fonction de diffusion dans le modele seulement
double
black_scholes::evaluate_mc() const
{
	const numerical_parameters_mc& params = dynamic_cast<const numerical_parameters_mc&>(numerical_parameters_);
	double sum_payoffs = 0.0;
	for (int simul = 0; simul < params.simuls_; ++simul)
	{
		double St = product_.get_fx().get_spot();
		for (int i = 0; i < t_.size() - 1; ++i)
		{
			double dt = t_[i + 1] - t_[i];
			// on diffuse le spot entre t[i] et t[i+1]


		}
		sum_payoffs += product_.payoff(St);
	}
	return (sum_payoffs / params.simuls_) * basis_.get_df(product_.get_expiry()) ;
}
