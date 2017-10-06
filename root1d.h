// root1d.h - one dimensional root finding
#pragma once
#include "ensure.h"
#include <functional>

namespace fms {
    namespace root1d {
        template<class X>
        inline auto bisect(X x0, X x1)
        {
            return (x0 + x1)/2;
        }
        // y = y0 + (x - x0)(y1 - y0)/(x1 - x0) = 0
        // x = x0 - y0 (x1 - x0)/(y1 - y0)
        //   = x0(y1 - y0)/(y1 - y0) - y0 (x1 - x0)/(y1 - y0)
        //   = (x0 y1 - y0 x1)/(y1 - y0)
        template<class X, class Y>
        inline auto secant(X x0, Y y0, X x1, Y y1)
        {
            ensure (y1 != y0);

            return (x0*y1 - y0*x1)/(y1 - y0);
        }
        // y = y0 + (x - x0)dy = 0
        // x = x0 - y0/dy
        template<class X, class Y>
        inline auto newton(X x0, Y y0, Y dy)
        {
            return x0 - y0/dy;
        }
        //!!! implement a one dimensional root finder
        template<class X, class Y>
        inline X newton_solve(X x, const std::function<Y(X)>& f, const std::function<Y(X)>& df)
        {
            // Use Newton's method.
            X x_ = newton(x, f(x), df(x));
            // Stop when 1 + (x_ - x) == 1.
            // while (something) {
            //   next guess
            // }
            
            return x_;
        }
    } // root1d
} // fms
