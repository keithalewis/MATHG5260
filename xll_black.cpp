// xllblack.cpp - Fischer Black forward value
#include "fms_black.h"
#include "G5260.h"

using namespace xll;
using namespace fms;

AddIn xai_black_put(
    Function(XLL_DOUBLE, L"?xll_black_put", L"BLACK.PUT")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Category(CATEGORY)
    .FunctionHelp(L"Forward value of a put option.")
);
double WINAPI xll_black_put(double f, double sigma, double k, double t)
{
#pragma XLLEXPORT
    double result = std::numeric_limits<double>::quiet_NaN();

    try {
        result = black::put(f, sigma, k, t);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}

#ifdef _DEBUG

xll::test xll_test_black_put([]() {
    black::test_put();
});

#endif // _DEBUG

AddIn xai_black_put_vega(
    Function(XLL_DOUBLE, L"?xll_black_put_vega", L"BLACK.PUT.VEGA")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Category(CATEGORY)
    .FunctionHelp(L"Derivative with respect to volaility of the forward value of a put option.")
);
double WINAPI xll_black_put_vega(double f, double sigma, double k, double t)
{
#pragma XLLEXPORT
    double result = std::numeric_limits<double>::quiet_NaN();

    try {
        result = black::put_vega(f, sigma, k, t);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}

#ifdef _DEBUG

xll::test xll_test_black_put_vega([]() {
    black::test_put_vega();
});

#endif // _DEBUG

AddIn xai_black_put_implied_volatility(
    Function(XLL_DOUBLE, L"?xll_black_put_implied_volatility", L"BLACK.PUT.IMPLIED.VOLATILITY")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"p", L"is the put price.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return the volatility that recovers the given put price.")
);
double WINAPI xll_black_put_implied_volatility(double f, double p, double k, double t)
{
#pragma XLLEXPORT
    double result = std::numeric_limits<double>::quiet_NaN();

    try {
        result = black::put_implied_volatility(f, p, k, t, .2);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}
