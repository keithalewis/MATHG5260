// bell.h - Bell polynomials
// B_{n+1}(x_1,...,x_{n+1}) = sum_0^n C(n,k) B_{n-k}(x_1,...,x_{n-k}) x_{k+1}
// and B_0 = 1
// B_1 = 1 B_0 x_1 = x_1
// B_2 = 1 B_1 x_1 + 1 B_0 x_2 = x_1^2 + x_2
// B_3 = 1 B_2 x_1 + 2 B_1 x_2 + 1 B_0 x_3
#pragma once

namespace fms {

    //!!! Return B_m(x[0],...,x[n-1])
    template<class X>
    inline X Bell(size_t m, size_t n, const X* x)
    {
    }

    
#ifdef _DEBUG

    inline void fms_bell_test(void)
    {
        //!!! Add tests for Bell polynomials.
        //!!! for m = 0,...,4
        //!!! and x = {.1,.01,001,.0001}
    }

#endif // _DEBUG


} // fms