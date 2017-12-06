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
		size_t i = std::lower_bound(t, t + n, u) - t;

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

		return f[i0] + m*(u - t[i0]);
	}

    // piecewise linear curve
	template<class T, class F>
	inline F slope(const T& u, size_t n, const T* t, const F* f)
	{
        ensure (n >= 2);

        size_t i0, i1;
		size_t i = std::lower_bound(t, t + n, u) - t;

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

		return m;
	}

    // Fit a function to cash, puts, calls, and a futures using
    // f(v) = f(u) 
    //      + int_0^u f''(k) (k - v)^+ dk 
    //      + int_u^infty f''(k) (v - k)^+ dk 
    //      + f'(u) (v - u).
    // Given a point u, fu = f(u),
    // k[i] and values f[i] of the function at k[i], i < n,
    // and dfu = f'(u)
    // find c[0] = f, c[1],..., c[n-2], c[n-1] = dfu such that
    // g(k[i]) = f[i] for i = 0,...,n - 1 where
    // g(v) = fu 
    //      + sum_{i > 0, k[i] < u} c[i] (k[i] - v)^+ 
    //      + sum_{i < n - 1, k[i] > u} c[i] (v - k[i])^+
    //      + dfu (v - u) 
    // Assume c points to an array with at least n elements.
#pragma warning(disable: 4100)
    template<class X = double>
    inline void fit(size_t n, const X* k, const X* f, X u, X fu, X dfu, X* c)
    {
        for (size_t i = 1; i < n; ++i) {
            c[i] = slope((k[i-1]+k[i])/2, n, k, f);
        }
        for (size_t i = 1; i < n - 1; ++i) {
            c[i] = c[i + 1] - c[i];
        }

        c[0] = fu;
        c[n-1] = dfu;
    }

    // Evaluate g(v) = c[0] + (sum puts < u) + (sum calls > u) + c[n-1](v - u)
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

inline void test_fms_pwlinear_value()
{
    using fms::pwlinear::value;
    using fms::pwlinear::slope;

    double k[] = {0, 1, 2, 3, 4};
    double f[] = {0, 0, 1, 0, 0};
    size_t n = sizeof(k)/sizeof(*k);

    ensure (value(0., n, k, f) == 0);
    ensure (value(1., n, k, f) == 0);
    ensure (value(2., n, k, f) == 1);
    ensure (value(3., n, k, f) == 0);
    ensure (value(4., n, k, f) == 0);

    ensure (value(0.5, n, k, f) == 0);
    ensure (value(1.5, n, k, f) == 0.5);
    ensure (value(2.5, n, k, f) == 0.5);
    ensure (value(3.5, n, k, f) == 0);
    ensure (value(4.5, n, k, f) == 0);

    ensure (slope(0.5, n, k, f) == 0);
    ensure (slope(1.5, n, k, f) == 1);
    ensure (slope(2.5, n, k, f) == -1);
    ensure (slope(3.5, n, k, f) == 0);
    ensure (slope(4.5, n, k, f) == 0);
}

inline void test_fms_pwlinear_val()
{
    // Butterfly spread.
    // u = 0, f(v) = 0, 0 <= v < 1
    //             = v - 1, 1 <= v < 2
    //             = 1 - (v - 2), 2 <= v < 3
    //             = 0, v >= 3
    // f(u) = f'(u) = 0 and
    // f'(v) = 0, 0 <= v < 1
    //       = 1, 1 <= v < 2
    //       = -1, 2 <= v < 3
    //       = 0, v >= 3
    // so f''(1) = delta(1), f''(2) = -2delta(2) and f''(3) = delta(3)
    // where delta(w) is a delta function having mass 1 at w.

    // pwlinear curve
    double k[] = {0, 1, 2, 3, 4};
    double f[] = {0, 0, 1, 0, 0};
    double c[] = {0, 1, -2, 1, 0}; // f(0), buy a call, sell a call, f'(0).
    auto F = [k,f](double v) { return fms::pwlinear::value(v, 5, k, f);};
    auto G = [k,c](double v) { return fms::pwlinear::val(v, 0., 5, k, c);};
    for (double v = 0.1; v <= 4; v += 1) {
        ensure (F(v) == G(v));
    }
}

inline void test_fms_pwlinear()
{
    double k[] = {1,2,3,4};
    size_t n = sizeof(k)/sizeof(k[0]); // dimof(k)
    double f[4]; // f(x) = x^2
    for (size_t i = 0; i < n; ++i) {
        f[i] = k[i]*k[i];
    }
    double c[4];
    for (double u = 1.1; u < 4; u += 1.1) {
        double fu = fms::pwlinear::value(u, n, k, f);
        double dfu = fms::pwlinear::slope(u, n, k, f);
        fms::pwlinear::fit(n, k, f, u, fu, dfu, c);
        for (size_t i = 0; i < n; ++i) {
            double v = k[i];
// This will fail until you implement pwlinear::fit.
            ensure (fms::pwlinear::val(v, u, n, k, c) == f[i]);
        }
    }
}

#endif // _DEBUG
