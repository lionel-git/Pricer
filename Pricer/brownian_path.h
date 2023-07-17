#pragma once
#include "fct2v.h"

#include <vector>

class brownian_path
{
public:
    // generate brownian path from 0 to T with N intervals (N+1 points)
    brownian_path(size_t N, double T);

    double integrate(size_t n, const fct2v& f);
    double integrate_sto(size_t n, const fct2v& f);

    size_t get_nb_iter(size_t step_size) const;

    double get_last_w() const { return *w_.rbegin(); }

private:
    size_t N_;
    double dt_;
    std::vector<double> w_;
};
