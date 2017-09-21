// sort.cpp - std::sort from the STL
//!!! Write xll_sort that calls std::sort and hook it up to XLL.SORT in Excel.

// unique.cpp - std::unique from the STL

#include "G5260.h"

using namespace xll;

AddIn xai_sort(
	Function(XLL_LPOPER, L"?xll_sort", L"XLL.SORT")
	.Arg(XLL_LPOPER, L"range", L"is a range.")
	.FunctionHelp(L"Sort range.")
	.Category(CATEGORY)
);
LPOPER WINAPI xll_sort(LPOPER po)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		std::sort(po->begin(), po->end());					//sort the range that po points to
		o.resize(1, std::distance(po->begin(), po->end()));
		std::copy(po->begin(), po->end(), o.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = OPER(xlerr::NA);
	}

	return &o;
}

