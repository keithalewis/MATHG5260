// final.cpp - G5260 final exam
#include "fms_black.h"
#include "fms_prob.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

// A _binary call option_ with strike k and maturity t pays b(x) = 1(x > k) at t.
// Assume call options are available at strikes k_ and _k where k_ < k < _k
// having prices c_ and _c respectively. Write a function that gives the
// price of an option with payoff 0 for x <= k_, 1 for x >= _k and is
// linear from (k_, 0) to (_k, 1).

//!!! Document the formula you use to price the approximation to the binary option.
// The first derivative is 1(k_ < x < _k)/(_k - k_) so the second derivative
// is 1/dk delta(k_) - 1/dk delta(_k), where dk = _k - k_.

#pragma warning(disable: 4100)
inline double approximate_binary(double k_, double c_, double _k, double _c)
{
    ensure (k_ < _k);
    ensure (c_ > _c);

    return (c_ - _c)/(_k - k_);
}

// The forward value of a binary call is E 1(F > k) = P(Z > z) where
// z = (s^2/2 + log(k/f))/s, F = f exp(sZ - s^2/2), and Z is standard normal.
// Use fms::black::call to calculate the difference between the approximate
// and the true value using f = k = 100, s = 0.01, k_ = 99, and _k = 101.

xll::test approximate_binary_test([]() {
    double f = 100, k = 100, s = 0.01, k_ = 99, _k = 101;
    double c_ = black::call(f, s, k_, 1);
    double _c = black::call(f, s, _k, 1); //!!! Black call at _k
    double ab = approximate_binary(k_, c_, _k, _c);
    double z = s/2 + log(k/f)/s;
    double bc = 1 - prob::normal::cdf(z);
    double eps = 0.00078486963284851718; 
    ensure (ab - bc == eps);
});

// The Bachelier model posits the stock price at time t is F_t = f + sigma B_t. 
// The forward call value is E max{F_t - k, 0} = (f - k) N(d) + sigma sqrt(t) n(d),
// where d = (f - k)/(sigma sqrt(t)), N is the standard normal cumulative distribution
// and n is the standard normal probability density function.

//!!! Implement a call valuation formula
inline double bachelier_call(double f, double sigma, double k, double t)
{
    double srt = sigma*sqrt(t);
    double d = (f - k)/srt;

    return (f - k)*prob::normal::cdf(d) + srt*prob::normal::pdf(d);
}

//!!! Implement the Excel add-in function "BACHELIER.CALL"
AddIn xai_bachelier_call(
    Function(XLL_DOUBLE, L"?xll_bachelier_call", L"BACHELIER.CALL")
    .Arg(XLL_DOUBLE, L"f", L"is there spot.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is the strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the time in years to maturity.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return the Bachelier call value")
);
double WINAPI xll_bachelier_call(double f, double sigma, double k, double t)
{
#pragma XLLEXPORT
    return bachelier_call(f, sigma, k, t);
}

//!!! Use xll_monte to create a test showing the formula is correct.