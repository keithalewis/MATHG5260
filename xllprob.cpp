// xllprob.cpp - probability functions
#include "prob.h"
#include "xll/xll.h"

using namespace fms;
using namespace xll;

AddIn xai_normal_pdf(
    Function(XLL_DOUBLE, L"?xll_normal_pdf", L"PROB.NORMAL.PDF")
    .Arg(XLL_DOUBLE, L"x", L"is a number")
    .Category(L"FMS")
    .FunctionHelp(L"Return the standard normal probability density function.")
);
double WINAPI xll_normal_pdf(double x)
{
#pragma XLLEXPORT
    double result;
    try {
        result = prob::normal::pdf(x);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return result;
}
/*
#ifdef _DEBUG

xll::test test_normal_pdf([]() {
    double x = xll_normal_pdf(0);
    double x_ = 1/sqrt(2*M_PI);
    ensure (x == x_);
});

#endif // _DEBUG
*/

AddIn xai_normal_cdf(
    Function(XLL_DOUBLE, L"?xll_normal_cdf", L"PROB.NORMAL.CDF")
    .Arg(XLL_DOUBLE, L"x", L"is a number")
    .Category(L"FMS")
    .FunctionHelp(L"Return the standard normal cumulative distribution.")
);
double WINAPI xll_normal_cdf(double x)
{
#pragma XLLEXPORT
    double result;

    try {
        result = prob::normal::cdf(x);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return result;
}
