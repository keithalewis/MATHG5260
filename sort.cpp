// sort.cpp - std::sort from the STL
//!!! Write xll_sort that calls std::sort and hook it up to XLL.SORT in Excel.

#include "G5260.h"

using namespace xll;

AddIn xai_sort(
	Function(XLL_LPOPER, L"?xll_sort", L"XLL.SORT")
	.Arg(XLL_LPOPER, L"range", L"is a range.")
	.FunctionHelp(L"Sort entries in a range.")
	.Category(CATEGORY)
);
LPOPER WINAPI xll_sort(LPOPER po)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		//notes
		//sorts the data in the input vector
		std::sort(po->begin(), po->end());
		//adjusts the size of the output vector to match the size of the input vector
		//by default the output will be a single cell, not a vector
		o.resize(1, std::distance(po->begin(), po->end()));
		//pastes the sorted vector into the output
		std::copy(po->begin(), po->end(), o.begin());
		//wish I knew how to display data as a column and not a row!
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = OPER(xlerr::NA);
	}

	return &o;
}