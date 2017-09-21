// sort.cpp - std::sort from the STL
//!!! Write xll_sort that calls std::sort and hook it up to XLL.SORT in Excel.
#include "G5260.h"

using namespace xll;

AddIn xai_sort(
	Function(XLL_LPOPER, L"?xll_sort", L"XLL.SORT")
	.Arg(XLL_LPOPER, L"range", L"is a range.")
	.FunctionHelp(L"Sort numerical entries from selected range based on acsending order.")
	.Category(CATEGORY)
);
LPOPER WINAPI xll_sort(LPOPER po)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		//Use std::sort from the STL to sort the selected range
		std::sort(po->begin(), po->end());
		//Get the length of the selected range
		int size = std::distance(po->begin(), po->end());
		//Resize the desingated column into appropriate length
		if (po->rows() == 1) {
			o.resize(1,size);
		}
		else {
			o.resize(size, 1);
		}
		//Cope the sorted entries into the desingated column
		std::copy(po->begin(), po->end(), o.begin());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = OPER(xlerr::NA);
	}
	return &o;
}
