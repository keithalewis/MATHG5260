// xllprob.cpp - probability functions
#include "prob.h"
#include "xll/xll.h"

using namespace fms;
using namespace xll;

AddIn xai_normal_pdf(
    Function(XLL_DOUBLE, L"?xll_normal_pdf", L"NORMAL.PDF")
    .Arg(XLL_DOUBLE, L"x", L"is a number")
    .Category(L"FMS")
    .FunctionHelp(L"Return the standard normal probability density function.")
);
double WINAPI xll_normal_pdf(double x)
{
#pragma XLLEXPORT
    return prob::normal::pdf(x);
}