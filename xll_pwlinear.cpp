// xll_pwlinear.cpp
#include "fms_pwlinear.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

xll::test xll_test_pwlinear([]() {
    test_fms_pwlinear_value();
    test_fms_pwlinear_val();
    test_fms_pwlinear();
});
