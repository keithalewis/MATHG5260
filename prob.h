// prob.h - Probability formulas
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace fms {
    namespace prob {
        struct normal {
            // probability distribution function
            static double pdf(double x)
            {
                static double _2pi = 2 * M_PI;

                return exp(-x*x/2)/_2pi;
            }
            // cumulative distribution function
            static double cdf(double x)
            {
                return std::erf(x/std::sqrt(2)); 
            }
        };
    } // prob
} // fms
