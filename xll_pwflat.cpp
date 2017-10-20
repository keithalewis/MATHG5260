// xll_pwflat.cpp - piecewise flat curves
#include "fms_pwflat.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

static AddIn xai_pwflat_curve(
    Function(XLL_HANDLE, L"?xll_pwflat_curve", L"PWFLAT.CURVE")
    .Arg(XLL_FP, L"times", L"is an array of times.")
    .Arg(XLL_FP, L"rates", L"is an array of rates.")
    .Uncalced() // <--- must have for handle classes
    .Category(CATEGORY)
    .FunctionHelp(L"Return a handle to a piecewise flat curve.")
);
HANDLEX WINAPI xll_pwflat_curve(const _FP12* pt, const _FP12* pr)
{
#pragma XLLEXPORT
    handlex h;

    try {
        ensure (size(*pt) == size(*pr));
        ensure (pwflat::monotonic(begin(*pt), end(*pt)));

        xll::handle<pwflat::interface<>> h_(new pwflat::curve<>(size(*pt), pt->array, pr->array));
        h = h_.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return h;
}

static AddIn xai_pwflat_value(
    Function(XLL_FP, L"?xll_pwflat_value", L"PWFLAT.VALUE")
    .Arg(XLL_HANDLE, L"handle", L"is a handle to a piecewise flat curve.")
    .Arg(XLL_FP, L"times", L"is an array of times at which to evaluate the curve.")
    .Arg(XLL_DOUBLE, L"extrapolate", L"is an optional rate to extrapolate the curve.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return the value of a piecewise flat curve at times.")
);
_FP12* WINAPI xll_pwflat_value(HANDLEX h, const _FP12* pt, double _f)
{
#pragma XLLEXPORT
    static xll::FP12 f;

    try {
        xll::handle<pwflat::interface<>> h_(h);
        ensure (h_);
        auto n = size(*pt);
        if (pt->rows == 1)
            f.resize(1, n);
        else
            f.resize(pt->rows, pt->columns);

        if (_f == 0) {
            _f = std::numeric_limits<double>::quiet_NaN();
        }

        for (int i = 0; i < n; ++i) {
            f[i] = h_->value(pt->array[i], _f);
        }
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return f.get();
}