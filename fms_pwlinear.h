// fms_pwlinear.h - piecewise linear and continuous curve
#pragma once
#include <algorithm> // adjacent_find
#include <numeric>   // upper/lower_bound
#include "ensure.h"

// Piecewise linear and continuous given points t_i, f_i.
namespace fms {
namespace pwlinear {

    // piecewise linear curve
	template<class T, class F>
	inline F value(const T& u, size_t n, const T* t, const F* f)
	{
        ensure (n >= 2);

        size_t i0, i1;
		auto i = std::lower_bound(t, t + n, u) - t;

        if (i == n) {
            i0 = n - 2;
            i1 = n - 1;
        }
        else if (i == 0) {
            i0 = 0;
            i1 = 1;
        }
        else {
            i0 = i - 1;
            i1 = i;
        }

        auto m = (f[i0] - f[i1])/(t[i0] - t[i1]);

		return f[i0] + m*(u - i0);
	}

    // Fit a function to cash, puts, calls, and a futures using
    // f(x) = f(u) + int_0^u f''(k) (k - x)^+ dk 
    //         + int_u^infty f''(k) (x - k)^+ dk + f'(u) (x - u).
    // Given k[0],...,k[n-1], values f[i] of the function at k[i],
    // a point u, fu = f(u), and dfu = f'(u)
    // find c[0] = a, c[1],..., c[n-2], c[n-1] = b such that
    // g(k[i]) = f[i] for i = 0,...,n - 1 where
    // g(v) = a + b (v - u) + sum_{i > 0, k[i] < u} c[i] (k[i] - v)^+ 
    //        + sum_{i < n - 1, k[i] > u} c[i] (v - k[i])^+
    // Assume c points to an array with at least n elements.
#pragma warning(disable: 4100)
    template<class X = double>
    inline void fit(size_t n, const X* k, const X* f, X u, X fu, X dfu, X* c)
    {
        //!!!Implement
    }
    // Evaluate g(v) = c[0] + sum puts + sum calls + c[n-1](v - u)
    template<class X = double>
    inline X val(X v, X u, size_t n, const X* k, const X* c)
    {
        ensure (n >= 2);

        X g = c[0] + c[n-1]*(v - u);
    
        for (size_t j = 1; j < n - 1 && k[j] < u; ++j)
            g += c[j]*std::max(k[j] - v, 0.);
        
        for (size_t j = n - 2; j > 0 && k[j] > u; --j)
            g += c[j]*std::max(v - k[j], 0.);

        return g;
    }

} // pwflinear
} // fms

#ifdef _DEBUG

inline void test_fms_pwlinear()
{
    double k[] = {1,2,3,4};
    size_t n = sizeof(k)/sizeof(k[0]); // dimof(k)
    double f[4]; // f(x) = x^2
    for (size_t i = 0; i < n; ++i)
        f[i] = k[i]*k[i];
    double c[4];
    for (double u = 1.2; u < 4; u += 1.1) {
        double fu = u*u;
        double dfu = 2*u;
        fms::pwlinear::fit(n, k, f, u, fu, dfu, c);
        for (size_t i = 0; i < n; ++i) {
            double v = k[i];
            ensure (fms::pwlinear::val(u, v, n, k, c) == f[i]);
        }
    }
}

#endif // _DEBUG
