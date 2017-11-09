// xll_range.cpp - Range operations
#include "G5260.h"

using namespace xll;

static AddIn xai_range_set(
    Function(XLL_HANDLE, L"?xll_range_set", L"RANGE.SET")
    .Arg(XLL_LPOPER, L"range", L"is a range of cells.")
    .Uncalced()
    .Category(CATEGORY)
    .FunctionHelp(L"Return a handle to a range of cells.")
);
HANDLEX WINAPI xll_range_set(LPOPER po)
{
#pragma XLLEXPORT
    xll::handle<OPER> ho(new OPER(*po));
    return ho.get();
}

static AddIn xai_range_get(
    Function(XLL_LPOPER, L"?xll_range_get", L"RANGE.GET")
    .Arg(XLL_HANDLE, L"handle", L"is a handle to a range of cells returned.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return a range of cells given a handle.")
);
LPOPER WINAPI xll_range_get(HANDLEX ho)
{
#pragma XLLEXPORT
    xll::handle<OPER> h(ho);
    
    return h.ptr();
}

static AddIn xai_range_mask(
    Function(XLL_LPOPER, L"?xll_range_mask", L"RANGE.MASK")
    .Arg(XLL_LPOPER, L"range", L"is a range of cells returned.")
    .Arg(XLL_LPOPER, L"mask", L"is a range of the same shape to be used as a mask.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return a range of cells not in the mask.")
);
LPOPER WINAPI xll_range_mask(LPXLOPER12 pr, LPXLOPER12 pm)
{
#pragma XLLEXPORT
    static OPER o;

    try {
        ensure (pr->val.array.rows == pm->val.array.rows);
        ensure (pr->val.array.columns == pm->val.array.columns);

        o.resize(pr->val.array.rows, pr->val.array.columns);
        int j = 0;
        for (int i = 0; i < o.size(); ++i) {
            if (!OPER(pm->val.array.lparray[i])) {
                o[j++] = pr->val.array.lparray[i];
            }
        }
        if (pr->val.array.columns == 1)
            o.resize(j, 1);
        else
            o.resize(1, j);

    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        o = OPER(xlerr::NA);
    }

    return &o;
}

static AddIn xai_range_push_back(
    Function(XLL_LPOPER, L"?xll_range_push_back", L"RANGE.STACK")
    .Arg(XLL_LPOPER, L"range", L"is a range of cells.")
    .Arg(XLL_LPOPER, L"range", L"is a range of cells.")
    .Arg(XLL_LPOPER, L"range", L"is a range of cells.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return ranges joined together.")
);
LPOPER WINAPI xll_range_push_back(const LPOPER po0, const LPOPER po1, const LPOPER po2)
{
#pragma XLLEXPORT
    static OPER o;

    try {
        o = *po0;
        if (*po1) {
            o.push_back(*po1);
            if (*po2) {
                o.push_back(*po2);
            }
        }
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        o = OPER(xlerr::NA);
    }

    return &o;
}

static AddIn xai_range_make(
    Function(XLL_LPOPER, L"?xll_range_make", L"RANGE.MAKE")
    .Arg(XLL_WORD, L"rows", L"is the number of rows in the returned range.")
    .Arg(XLL_WORD, L"columns", L"is a numberg of columns in the returned range.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return ranges with given rows and columns.")
);
LPOPER WINAPI xll_range_make(WORD rows, WORD columns)
{
#pragma XLLEXPORT
    static OPER o;

    try {
        o.resize(rows, columns);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        o = OPER(xlerr::NA);
    }

    return &o;
}

/*
static Auto<Close> xac_handle_oper([]() {
    xll::handle<OPER>::gc();

    return TRUE;
});
*/