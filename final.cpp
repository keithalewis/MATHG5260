// final.cpp - G5260 final exam
#include "G5260.h"
//!!! include other appropriate header files

using namespace xll;

// A _binary call option_ with strike k and maturity t pays b(x) = 1(x > k) at t.
// Assume call options are available at strikes k_ and _k where k_ < k < _k
// having prices c_ and _c respectively. Write a function that gives the
// price of an option with payoff 0 for x <= k_, 1 for x >= _k and is
// linear from (k_, 0) to (_k, 1).

//!!! Document the formula you use to price the approximation to the binary option.

#pragma warning(disable: 100)
inline double approximate_binary(double k_, double c_, double _k, double _c)
{
    return 0; //!!! implement here
}

// The forward value of a binary call is E 1(F > k) = P(Z > z) where
// z = (s^2/2 + log(k/f))/s, F = f exp(sZ - s^2/2), and Z is standard normal.
// Use fms::black::call to calculate the difference between the approximate
// and the true value using f = k = 100, s = 0.01, k_ = 99, and _k = 101.

xll::test approximate_binary_test([]() {
    double f = 100, k = 100, s = 0.01, k_ = 99, _k = 101;
    double c_ = 0; //!!! Black call at k_
    double _c = 0; //!!! call at _k
    double ab = approximate_binary(k_, c_, _k, _c);
    double bc = 0; //!!! formula for binary call
    double eps = 0; //!!! find appropriate value
    ensure (ab - bc == eps);
});

// The Bachelier model posits the stock price at time t is F_t = f + sigma B_t. 
// The forward call value is E max{F_t - k, 0} = (f - k) N(d) + sigma sqrt(t) n(d),
// where d = (f - k)/(sigma sqrt(t)), N is the standard normal cumulative distribution
// and n is the standard normal probability density function.

//!!! Implement a call valuation formula
inline double bachelier_call(double f, double sigma, double k, double t)
{
    return 0;
}

//!!! Implement the Excel add-in function "BACHELIER.CALL"

//!!! Use xll_monte to create a test showing the formula is correct.