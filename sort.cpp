// sort.cpp - std::sort from the STL
#include <algorithm>
#include <xutility>
#include "G5260.h"

using namespace xll;

AddIn xai_sort(
    Function(XLL_LPOPER, L"?xll_sort", L"XLL.SORT")
    .Arg(XLL_LPOPER, L"range", L"is a range.")
    .FunctionHelp(L"Sort entries from range.")
    .Category(CATEGORY)
);
LPOPER WINAPI xll_sort(LPOPER po)
{
#pragma XLLEXPORT
    static OPER o;

    try {
        std::sort(po->begin(), po->end());
        if (po->rows() == 1) {
            o.resize(1, po->columns());
        }
        else {
            o.resize(po->rows(), po->columns());
        }
        std::copy(po->begin(), po->end(), o.begin());
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        o = OPER(xlerr::NA);
    }

    return &o;
}

