// xll_njr.cpp - Normal Jarrow-Rudd model
#include "fms_njr.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

Auto<Open> xao_fms_choose_test([]() { 
    fms_choose_test();
    fms_hermite_test();
    fms_bell_test();

    return TRUE; 
});

AddIn xai_njr_put(
    Function(XLL_DOUBLE, L"?xll_njr_put", L"NJR.PUT")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Arg(XLL_FP, L"kappa", L"is the perturbation of the cumulants from a standard normal.")
    .Category(CATEGORY)
    .FunctionHelp(L"Forward value of a put option.")
);
double WINAPI xll_njr_put(double f, double sigma, double k, double t, const _FP12* pkappa)
{
#pragma XLLEXPORT
    double result = std::numeric_limits<double>::quiet_NaN();

    try {
        result = njr::put(f, sigma, k, t, size(*pkappa), pkappa->array);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}

AddIn xai_njr_call(
    Function(XLL_DOUBLE, L"?xll_njr_call", L"NJR.CALL")
    .Arg(XLL_DOUBLE, L"f", L"is the forward value.")
    .Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
    .Arg(XLL_DOUBLE, L"k", L"is strike.")
    .Arg(XLL_DOUBLE, L"t", L"is the expiration in years.")
    .Arg(XLL_FP, L"kappa", L"is the perturbation of the cumulants from a standard normal.")
    .Category(CATEGORY)
    .FunctionHelp(L"Forward value of a call option.")
);
double WINAPI xll_njr_call(double f, double sigma, double k, double t, const _FP12* pkappa)
{
#pragma XLLEXPORT
    double result = std::numeric_limits<double>::quiet_NaN();

    try {
        result = njr::call(f, sigma, k, t, size(*pkappa), pkappa->array);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}

