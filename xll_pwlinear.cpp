// xll_pwlinear.cpp
#include "fms_pwlinear.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

static Auto<Open> xao_test_fms_pwlinear([]() {
    test_fms_pwlinear();
    return TRUE;
});
