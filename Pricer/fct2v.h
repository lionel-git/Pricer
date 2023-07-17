#pragma once

class fct2v
{
public:
    virtual double evaluate(double t, double x) const = 0;
    virtual double IntegralSto(double t, double x) const = 0;
};