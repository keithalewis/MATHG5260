// bell.h - Bell polynomials
// B_{n+1}(x_1,...,x_{n+1}) = sum_0^n C(n,k) B_{n-k}(x_1,...,x_{n-k}) x_{k+1}
// and B_0 = 1
// B_1 = 1 B_0 x_1 = x_1
// B_2 = 1 B_1 x_1 + 1 B_0 x_2 = x_1^2 + x_2
// B_3 = 1 B_2 x_1 + 2 B_1 x_2 + 1 B_0 x_3
#pragma once
#include "ensure.h"
#include <vector>

namespace fms {

    // n choose k = n!/k!(n - k)!
    constexpr size_t choose(size_t n, size_t k)
    {
        ensure (k <= n);
        
        if (k == 0 || n == k)
           return 1;

        return choose(n - 1, k) + choose(n - 1, k - 1);
    }

    // B_n(x_1,...,x_n}) = sum_0^{n-1} C(n-1,k) B_{n-1-k}(x_1,...,x_{n-1-k}) x_{k+1}
    template<class X>
    inline X Bell(size_t n, const X* x)
    {
        if (n == 0)
            return 1;

        X B = 0;
        for (size_t k = 0; k < n; ++k) {
            B += choose(n - 1, k) * Bell(n - 1 - k, x)*x[k];
        }

        return B;
    }

    
#ifdef _DEBUG

    inline void fms_choose_test(void)
    {
        ensure (choose(0,0) == 1);
        ensure (choose(4,2) == 6);
    }
    
    inline void fms_bell_test(void)
    {
        double x[] = {.1,.01,001,.0001};
        ensure (Bell(0, x) == 1);
        ensure (Bell(1, x) ==   x[0]);
        ensure (Bell(2, x) ==   x[0]*x[0] + x[1]);
        ensure (Bell(3, x) ==  (x[0]*x[0] + x[1])*x[0] + 2*x[0]*x[1] + x[2]);
        ensure (Bell(4, x) == ((x[0]*x[0] + x[1])*x[0] + 2*x[0]*x[1] + x[2])*x[0]
                           + 3*(x[0]*x[0] + x[1])*x[1]
                           + 3*(x[0])*x[2] + x[3]);
    }

#endif // _DEBUG

} // fms