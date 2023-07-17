#include "lab.h"
#include "brownian_path.h"
#include "fct2v.h"

#include <iostream>

namespace lab
{
    class fct_one : public fct2v
    {
        virtual double evaluate(double /*t*/, double /*x*/) const
        {
            return 1.0;
        }

        virtual double IntegralSto(double /*t*/, double x) const
        {
            return x;
        }
    };

    class fct_x : public fct2v
    {
        virtual double evaluate(double /*t*/, double x) const
        {
            return x;
        }

        virtual double IntegralSto(double t, double x) const
        {
            return (x * x - t) / 2.0;
        }


    };

    void test_integral_fct(const fct2v& f)
    {
        std::cout << "start test_integral" << std::endl;

        size_t N = 1ul << 21;
        double T = 10.0;
        brownian_path b_path(N, T);
        size_t step_size = N;
        while (step_size >= 1)
        {
            std::cout << "Step size = " << step_size << std::endl;
            double s1 = b_path.integrate(step_size, f);
            std::cout << "Int = " << s1 << std::endl;
            double s2 = b_path.integrate_sto(step_size, f);
            std::cout << "Int Sto = " << s2 << std::endl;
            step_size >>= 1;
        }
        std::cout << "IntSto = " << f.IntegralSto(T, b_path.get_last_w()) << std::endl;
    }

    void test_integral()
    {
        std::cout << "start test_integral" << std::endl;

//        fct_one f1;
//        test_integral_fct(f1);

        fct_x fx;
        test_integral_fct(fx);


    }
}