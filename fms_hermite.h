// hermit.h - Hermite polynomials
// H_{n+1}(x) = x H_n(x)−n H_{n−1}(x), H_0(x) = 1, H_1(x) = x. 
#pragma once
namespace fms {

    // H_n(x) = x H_{n-1}(x) - (n - 1) H_{n-2}(x)
    template<class X>
    inline X Hermite(size_t n, X x)
    {
        if (n == 0)
            return X(1);
        if (n == 1)
            return x;

        return x*Hermite(n - 1, x) - (n - 1)*Hermite(n - 2, x);
    }

#ifdef _DEBUG

    inline void fms_hermite_test(void)
    {
        for (double x = -2; x < 2; x += 0.1) {
            ensure (Hermite(0, x) == 1);
            ensure (Hermite(1, x) == x);
            ensure (Hermite(2, x) == x*x - 1);
            // x*x*x - 3*x fails
            ensure (Hermite(3, x) == x*(x*x -1) - 2*x);
        }
    }

#endif // _DEBUG

} // fms
