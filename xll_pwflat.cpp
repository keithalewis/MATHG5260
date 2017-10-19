// xll_pwflat.cpp - piecewise flat curves
#include <vector>
#include "fms_pwflat.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

class pwflat_curve : public pwflat::curve<> {
    std::vector<double> t_, f_;
public:
    pwflat_curve()
    { }
    pwflat_curve(size_t n, const double* t, const double* f)
        : t_(t, t + n), f_(f, f + n)
    { }
private:
    size_t _size() const override
    {
        return t_.size();
    }
    const double* _time() const override
    {
        return t_.data();
    }
    const double* _rate() const override
    {
        return f_.data();
    }
};

static AddIn xai_pwflat_curve(
    Function(XLL_HANDLE, L"?xll_pwflat_curve", L"PWFLAT.CURVE")
    .Arg(XLL_FP, L"times", L"is an array of times.")
    .Arg(XLL_FP, L"rates", L"is an array of rates.")
    .Uncalced()
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

        xll::handle<pwflat::curve<>> h_(new pwflat_curve(size(*pt), pt->array, pr->array));
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
    .Category(CATEGORY)
    .FunctionHelp(L"Return the value of a piecewise flat curve at times.")
);
_FP12* WINAPI xll_pwflat_value(HANDLEX h, const _FP12* pu)
{
#pragma XLLEXPORT
    static xll::FP12 f;

    try {
        xll::handle<pwflat::curve<>> h_(h);
        ensure (h_);
        auto n = size(*pu);
        if (pu->rows == 1)
            f.resize(1, n);
        else
            f.resize(n, 1);
        for (int i = 0; i < n; ++i) {
            f[i] = h_->value(pu->array[i]);
        }
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return f.get();
}