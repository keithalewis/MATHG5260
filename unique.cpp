// unique.cpp - std::unique from the STL
#include <algorithm>
#include <xutility>
#include "G5260.h"

using namespace xll;

AddIn xai_unique(
    Function(XLL_LPOPER, L"?xll_unique", L"XLL.UNIQUE")
    .Arg(XLL_LPOPER, L"range", L"is a range.")
    .FunctionHelp(L"Remove adjacent duplicate entries from range.")
    .Category(CATEGORY)
);
LPOPER WINAPI xll_unique(LPOPER po)
{
#pragma XLLEXPORT
    static OPER o;

    try {
        auto e = std::unique(po->begin(), po->end());
        if (po->rows() == 1) {
            o.resize(1, (COL)std::distance(po->begin(), e));
        }
        else {
            o.resize((RW)std::distance(po->begin(), e), 1);
        }
        std::copy(po->begin(), e, o.begin());
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        o = OPER(xlerr::NA);
    }

    return &o;
}

