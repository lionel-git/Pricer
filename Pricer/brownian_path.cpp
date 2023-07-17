#include "brownian_path.h"
#include "brownian.h"
#include "pricer_exception.h"
#include "my_format.h"

#include <cmath>

brownian_path::brownian_path(size_t N, double T): N_(N), dt_(T/N)
{
    brownian b;
    w_.resize(N_ + 1);
    double sqrt_dt = std::sqrt(dt_);
    double w = 0.0;
    for (size_t i = 0; i <= N; i++)
    {
        w_[i] = w;
        w += b.normal_value() * sqrt_dt;
    }
}

size_t 
brownian_path::get_nb_iter(size_t step_size) const
{
    if (N_ % step_size != 0)
        THROW("Not a divisor");
    return N_ / step_size;
}

double 
brownian_path::integrate(size_t step_size, const fct2v& f)
{
    auto n = get_nb_iter(step_size);
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
        double value = f.evaluate(i * dt_, w_[i * step_size]);
        sum += value * step_size * dt_;
    }
    return sum;
}

double 
brownian_path::integrate_sto(size_t step_size, const fct2v& f)
{
    auto n = get_nb_iter(step_size);
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
        double value = f.evaluate(i * dt_, w_[i * step_size]);
        double dw = w_[(i + 1) * step_size] - w_[i * step_size];
        sum += value * dw;
    }
    return sum;
}
