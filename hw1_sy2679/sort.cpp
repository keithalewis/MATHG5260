// sort.cpp - std::sort from the STL
//!!! Write xll_sort that calls std::sort and hook it up to XLL.SORT in Excel.

#include "G5260.h"

using namespace xll;

AddIn xai_sort(
	Function(XLL_LPOPER, L"?xll_sort", L"XLL.SORT")
	.Arg(XLL_LPOPER, L"range", L"is a range.")
	.FunctionHelp(L"Sort the selected list in ascending order")
	.Category(CATEGORY)
);
LPOPER WINAPI xll_sort(LPOPER po)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		// First get the length of the input
		int leng = std::distance(po->begin(), po->end());

		// Then sort the input with std
		std::sort(po->begin(), po->end());

		// Perform Resize
		if (po->rows() == 1) {
			o.resize(1, leng);
		}
		else {
			o.resize(leng, 1);
		}
		
		// Output the sorted array
		std::copy(po->begin(), po->end(), o.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = OPER(xlerr::NA);
	}

	return &o;
}