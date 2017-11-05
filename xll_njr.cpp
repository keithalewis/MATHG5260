// xll_njr.cpp - Normal Jarrow-Rudd model
#include "fms_bell.h"
#include "fms_hermite.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

Auto<Open> xao_fms_choose_test([]() { fms_choose_test(); return TRUE; });
