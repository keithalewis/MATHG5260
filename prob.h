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
                static double sqrt_2pi = std::sqrt(2 * M_PI);

                return exp(-x*x/2)/sqrt_2pi;
            }
            // cumulative distribution function
            static double cdf(double x)
            {
                static double sqrt2 = std::sqrt(2);

                return 0.5 + std::erf(x/sqrt2)/2; 
            }
        };
    } // prob
} // fms
