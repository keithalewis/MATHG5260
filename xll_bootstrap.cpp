// xll_bootstrap.cpp - Bootstrap a curve.
#include "fms_bootstrap.h"
#include "G5260.h"

using namespace xll;
using namespace fms;

AddIn xai_xll_bootstrap(
    Function(XLL_DOUBLE, L"?xll_pwflat_bootstrap", L"PWFLAT.BOOTSTRAP")
    .Arg(XLL_DOUBLE, L"price", L"is the price of the instrument.")
    .Arg(XLL_FP, L"instrument", L"is a two row array of times and cash flows")
    .Arg(XLL_FP, L"curve", L"is a two row array of times and forwards")
    .Arg(XLL_DOUBLE, L"initial", L"is an initial guess at the bootstrap value.")
    .Category(CATEGORY)
    .FunctionHelp(L"Bootstrap a curve")
);
double WINAPI xll_pwflat_bootstrap(double p, _FP12* pi, _FP12* pc, double f)
{
#pragma XLLEXPORT
    try {
        ensure (pi->rows == 2);
        ensure (pc->rows == 2);
        auto m = pi->columns;
        auto n = pc->columns;

        f = pwflat::bootstrap(p, m, pi->array, pi->array + m, n, pc->array, pc->array +n, f);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        f = std::numeric_limits<double>::quiet_NaN();
    }

    return f;
}