// unique.cpp - std::unique from the STL
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
        o.resize(1, std::distance(po->begin(), e));
        std::copy(po->begin(), e, o.begin());
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        o = OPER(xlerr::NA);
    }

    return &o;
}

