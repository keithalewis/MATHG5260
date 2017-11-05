// xll_bootstrap.cpp - Bootstrap a curve.
#include "fms_bootstrap.h"
#include "G5260.h"

using namespace xll;
using namespace fms;

AddIn xai_xll_bootstrap(
    Function(XLL_FP, L"?xll_pwflat_bootstrap", L"PWFLAT.BOOTSTRAP")
    .Arg(XLL_DOUBLE, L"price", L"is the price of the instrument.")
    .Arg(XLL_FP, L"instrument", L"is a two row array of times and cash flows or a handle to a fixed income instrument.")
    .Arg(XLL_FP, L"curve", L"is a two row array of times and forwards or a handle to a pwflat curve.")
    .Arg(XLL_DOUBLE, L"initial", L"is an initial guess at the bootstrap value.")
    .Category(CATEGORY)
    .FunctionHelp(L"Bootstrap a curve")
);
_FP12* WINAPI xll_pwflat_bootstrap(double p, _FP12* pi, _FP12* pc, double f_)
{
#pragma XLLEXPORT
    static xll::FP12 tf(2,1);

    try {
        size_t m, n;
        const double *u, *c, *t, *f;

        if (size(*pi) == 1) {
            xll::handle<fixed_income::instrument<>> i_(pi->array[0]);
            ensure (i_);
            m = i_->size();
            u = i_->time();
            c = i_->cash();
        }
        else {
            ensure (pi->rows == 2);
            m = pi->columns;
            u = pi->array;
            c = pi->array + m;
        }
        if (size(*pc) == 1) {
            xll::handle<pwflat::curve<>> c_(pc->array[0]);
            ensure (c_);
            n = c_->size();
            t = c_->time();
            f = c_->rate();
        }
        else {
            ensure (pc->rows == 2);
            n = pc->columns;
            t = pc->array;
            f = pc->array + n;
        }

        std::tie(tf[0],tf[1]) = pwflat::bootstrap(p, m, u, c, n, t, f, f_);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return tf.get();
}