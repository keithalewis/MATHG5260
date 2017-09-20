// sort.cpp - std::sort from the STL
//!!! Write xll_sort that calls std::sort and hook it up to XLL.SORT in Excel.
#include "G5260.h"

using namespace xll;

AddIn xai_sort(
	Function(XLL_LPOPER, L"?xll_sort", L"XLL.SORT")
	.Arg(XLL_LPOPER, L"range", L"is a range.")
	.FunctionHelp(L"Sorts the elements in the range [first,last) into ascending order.")
	.Category(CATEGORY)
);
LPOPER WINAPI xll_sort(LPOPER po)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		auto e = std::sort(po -> begin(), po -> end());
		for (auto a : o) {
			std::cout << a << " ";
		}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = OPER(xlerr::NA);
	}

	return &o;
}
