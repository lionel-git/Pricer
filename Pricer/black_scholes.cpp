#include "black_scholes.h"
#include "fx_manager.h"
#include "numerical_parameters_edp.h"
#include "numerical_parameters_mc.h"
#include "brownian.h"
#include "matrix.h"
#include <cmath>

black_scholes::black_scholes(const product& product, const numerical_parameters& np) :
    model(product, np), vol_bs_(product_.get_fx().get_bs_vol())
{
    initialize();
    check_parameters();
}

void
black_scholes::check_parameters() const
{
    // some checks
    if (product_.get_strike() < -1e-10)
        THROW("Strike cannot be negative");
    if (vol_bs_ < -1e-10)
        THROW("Vol cannot be negative");
}

double
black_scholes::evaluate_closed_f() const
{
    double T = product_.get_expiry();
    double F = product_.get_fx().get_fwd(T);
    double K = std::max(1e-10, product_.get_strike());
    double vol_time = std::max(1e-10, vol_bs_ * sqrt(T));

    double d_plus = std::log(F / K) / vol_time + 0.5 * vol_time;
    double d_minus = d_plus - vol_time;

    if (product_.get_sign() > 0)
        return (brownian::cdf(d_plus) * F - brownian::cdf(d_minus) * K) * basis_.get_df(T); // call
    else if (product_.get_sign() < 0)
        return (brownian::cdf(-d_minus) * K - brownian::cdf(-d_plus) * F) * basis_.get_df(T); // put
    THROW("Bad sign");
}

void
black_scholes::get_edp_xbounds(double& x_min, double& x_max) const
{
    double T = product_.get_expiry();
    x_max = 0.0;
    double dt = T / 100.0;
    for (double t = 0.0; t <= T * (1.0 + 1e-6); t += dt)
    {
        double Ft = product_.get_fx().get_fwd(t);
        double vol_t = vol_bs_ * sqrt(t);
        x_max = std::max(x_max, Ft + 5 * Ft * vol_t);
    }
    x_max = std::max(x_max, product_.get_fx().get_spot() * 1.1);
    x_min = 0.0;
}

// input: V at t+dt
// U_up, U_down: V at t
//  u V    u=U_up
//  . V
//  . V    V connu, . = inconnu
//  . V
//  d V    d=U_down
void
black_scholes::back_propagate_explicit(std::vector<double>& V, double dt, double r, double U_up, double U_down) const
{
    // EDP: dV/dt + 0.5*s^2*S^2*d2V/dS2 + r.S.dV/dS - r.V = 0
    // dV/dt = r.V - 0.5*s^2*S^2*d2V/dS2 - r.S.dV/dS
    // V(t+dt)-V(t) = dt.(r.V - 0.5*s^2*S^2*d2V/dS2 - r.S.dV/dS )
    // V(t) = V(t+dt) - dt.(r.V - r.S.dV/dS - 0.5*s^2*S^2*d2V/dS2 )

    // Rem S=x[i], V and x have same length

    size_t N = V.size();
    std::vector<double> U(N); // U vector at t, V vector at t+dt
    U[N - 1] = U_up;
    U[0] = U_down;
    for (size_t i = 1; i <= N - 2; ++i)
    {

        double value0 = r * V[i];
        double value1 = -r * x_[i] * (edp_coeffs_[i].c1a_ * V[i - 1] + edp_coeffs_[i].c1b_ * V[i] + edp_coeffs_[i].c1c_ * V[i + 1]);
        double value2 = -0.5 * vol_bs_ * vol_bs_ * x_[i] * x_[i] * (edp_coeffs_[i].c2a_ * V[i - 1] + edp_coeffs_[i].c2b_ * V[i] + edp_coeffs_[i].c2c_ * V[i + 1]);
        U[i] = V[i] - dt * (value0 + value1 + value2);
    }
    V = U;
}

void
black_scholes::back_propagate_implicit(std::vector<double>& /*V*/, double /*dt*/, double /*r*/, double /*U_up*/, double /*U_down*/) const
{
}

void
black_scholes::back_propagate_cranck_nicholson(std::vector<double>& V, double dt, double r, double U_up, double U_down) const
{
    // matrix vectors
    size_t N = V.size();
    std::vector<double> a(N - 1); // lower diag: 2 => N-2
    std::vector<double> b(N - 1); // diag: 1 => N-2
    std::vector<double> c(N - 1); // upper diag: 1 => N-3
    std::vector<double> t(N - 1); // target vector: 1 => N-2

    // ========= GENERATED CODE ===========
    {
        size_t i = 1;
        double c1a = edp_coeffs_[i].c1a_;
        double c1b = edp_coeffs_[i].c1b_;
        double c1c = edp_coeffs_[i].c1c_;
        double c2a = edp_coeffs_[i].c2a_;
        double c2b = edp_coeffs_[i].c2b_;
        double c2c = edp_coeffs_[i].c2c_;
        t[i] = -(((1 - theta_) * (-dt * ((-r) * x_[i] * c1a * U_down + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2a * U_down)) + theta_ * (V[i] - dt * ((r * V[i] + (-r) * x_[i] * ((c1a * V[i - 1] + c1b * V[i]) + c1c * V[i + 1])) + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * ((c2a * V[i - 1] + c2b * V[i]) + c2c * V[i + 1])))));
        a[i] = 0;
        b[i] = ((1 - theta_) * (1 - dt * ((r + (-r) * x_[i] * c1b) + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2b)) - 1);
        c[i] = (1 - theta_) * (-dt * ((-r) * x_[i] * c1c + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2c));
    }
    {
        for (size_t i = 2; i <= N - 3; ++i) {
            double c1a = edp_coeffs_[i].c1a_;
            double c1b = edp_coeffs_[i].c1b_;
            double c1c = edp_coeffs_[i].c1c_;
            double c2a = edp_coeffs_[i].c2a_;
            double c2b = edp_coeffs_[i].c2b_;
            double c2c = edp_coeffs_[i].c2c_;
            t[i] = -(theta_ * (V[i] - dt * ((r * V[i] + (-r) * x_[i] * ((c1a * V[i - 1] + c1b * V[i]) + c1c * V[i + 1])) + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * ((c2a * V[i - 1] + c2b * V[i]) + c2c * V[i + 1]))));
            a[i] = (1 - theta_) * (-dt * ((-r) * x_[i] * c1a + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2a));
            b[i] = ((1 - theta_) * (1 - dt * ((r + (-r) * x_[i] * c1b) + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2b)) - 1);
            c[i] = (1 - theta_) * (-dt * ((-r) * x_[i] * c1c + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2c));
        }
    }
    {
        size_t i = N - 2;
        double c1a = edp_coeffs_[i].c1a_;
        double c1b = edp_coeffs_[i].c1b_;
        double c1c = edp_coeffs_[i].c1c_;
        double c2a = edp_coeffs_[i].c2a_;
        double c2b = edp_coeffs_[i].c2b_;
        double c2c = edp_coeffs_[i].c2c_;
        t[i] = -(((1 - theta_) * (-dt * ((-r) * x_[i] * c1c * U_up + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2c * U_up)) + theta_ * (V[i] - dt * ((r * V[i] + (-r) * x_[i] * ((c1a * V[i - 1] + c1b * V[i]) + c1c * V[i + 1])) + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * ((c2a * V[i - 1] + c2b * V[i]) + c2c * V[i + 1])))));
        a[i] = (1 - theta_) * (-dt * ((-r) * x_[i] * c1a + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2a));
        b[i] = ((1 - theta_) * (1 - dt * ((r + (-r) * x_[i] * c1b) + (-0.5) * vol_bs_ * vol_bs_ * x_[i] * x_[i] * c2b)) - 1);
        c[i] = 0;
    }
    // ========= END GENERATED CODE ===========






    auto x = matrix::solve_tridiagonal_system(a, b, c, t);
    V[0] = U_down;
    V[N - 1] = U_up;
    std::copy(x.begin()++, x.end(), V.begin()++);
}

double
black_scholes::get_dS_mc(double normal_value, double S, double dt) const
{
    // dS = S.s.dWt
    return S * vol_bs_ * normal_value * sqrt(dt);
}
