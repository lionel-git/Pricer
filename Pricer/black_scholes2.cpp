#include "black_scholes2.h"
#include "numerical_parameters_mc.h"
#include "brownian.h"

black_scholes2::black_scholes2(const product& product, const numerical_parameters& np): black_scholes(product, np)
{
    initialize();
    check_parameters();
}

void
black_scholes2::get_edp_xbounds(double& x_min, double& x_max) const
{
    black_scholes::get_edp_xbounds(x_min, x_max);
    // A revoir
    x_min = std::log(x_min);
    x_max = std::log(x_max);
}

std::set<double> 
black_scholes2::convert_strikes(const std::set<double>& strikes) const
{
    std::set<double> out;
    for (const auto strike : strikes)
        out.insert(std::log(strike));
    return out;
}

void
black_scholes2::back_propagate_explicit(std::vector<double>& V, double dt, double r, double U_up, double U_down) const
{

}

void
black_scholes2::back_propagate_cranck_nicholson(std::vector<double>& V, double dt, double r, double U_up, double U_down) const
{

}

void 
black_scholes2::initialize_terminal_payoff_and_bounds()
{
    V_terminal_.resize(x_.size());
    for (size_t i = 0; i < x_.size(); ++i)
        V_terminal_[i] = product_.payoff(std::exp(x_[i]));

    V_bound_up_.resize(t_.size());
    V_bound_down_.resize(t_.size());

    // On suppose que aux bornes up et down, la vol n'a plus d'importance sur le payoff
    // reste a determiner dW/du aux bornes
    // W(t) = W(t + dt) - dt.(r.W - r.dW / du)
    // W(t) = W(t + dt) - r.dt.(W - dW/du)
    V_bound_up_[t_.size() - 1] = V_terminal_[x_.size() - 1];
    V_bound_down_[t_.size() - 1] = V_terminal_[0];
    //double S_up = x_[x_.size() - 1];
    double dvds_up = (V_terminal_[x_.size() - 1] - V_terminal_[x_.size() - 2]) / (x_[x_.size() - 1] - x_[x_.size() - 2]);
    //double S_down = x_[0];
    double dvds_down = (V_terminal_[1] - V_terminal_[0]) / (x_[1] - x_[0]);
    for (size_t j = t_.size() - 1; j >= 1; --j)
    {
        double dt = t_[j] - t_[j - 1];
        double r = r_[j]; // rem r[0] est indefini
        V_bound_up_[j - 1] = V_bound_up_[j] - r * dt * (V_bound_up_[j] - dvds_up);
        V_bound_down_[j - 1] = V_bound_down_[j] - r * dt * (V_bound_down_[j] - dvds_down);
    }
}

double
black_scholes2::get_dS_mc(double normal_value, double /*log_S*/, double dt) const
{
    // dS/S = s.dWt
    // d(log(S)) = s.dWt
    return vol_bs_ * normal_value * sqrt(dt);
}

double
black_scholes2::evaluate_mc() const
{ 
    const numerical_parameters_mc& params = dynamic_cast<const numerical_parameters_mc&>(numerical_parameters_);
    double sum_payoffs = 0.0;
    brownian b;
    for (int simul = 0; simul < params.simuls_; ++simul)
    {
        // u = log(S)
        double ut = std::log(product_.get_fx().get_spot());
        for (size_t j = 1; j < t_.size(); ++j)
        {
            // on diffuse le spot entre t[j-1] et t[j]
            double dt = t_[j] - t_[j - 1];
            double du_deter = r_[j] * dt;
            double du_stochastic = get_dS_mc(b.normal_value(), ut, dt);
            ut += du_deter + du_stochastic;
        }
        sum_payoffs += product_.payoff(std::exp(ut - 0.5 * vol_bs_ * vol_bs_ * 1.0));  // A voir !!
    }
    return (sum_payoffs / params.simuls_) * basis_.get_df(product_.get_expiry());
}
