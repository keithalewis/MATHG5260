// xllblack.cpp - Fischer Black forward value
#include "black.h"
#include "xll/xll.h"

using namespace xll;
using namespace fms;

//!!! Implement the Excel add-in BLACK.PUT in the FMS category.
AddIn xai_black_put(
    Function(XLL_DOUBLE, L"?xll_black_put", L"BLACK.PUT")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Category(L"FMS")
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
    //!!! Add a test for xll_black_put here.
});

#endif // _DEBUG

//!!! Implement the Excel add-in BLACK.PUT in the FMS category.
AddIn xai_black_put_vega(
    Function(XLL_DOUBLE, L"?xll_black_put_vega", L"BLACK.PUT.VEGA")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Category(L"FMS")
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
