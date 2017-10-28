// root1d.h - one dimensional root finding
#pragma once
#include <functional>
#include <limits>
#include "ensure.h"
#include "ulp.h"

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

        template<class X, class Y>
        inline X newton_solve(X x, const std::function<Y(X)>& f, const std::function<Y(X)>& df, 
            size_t iter = 100, long long ulp = 10)
        {
            X x_ = newton(x, f(x), df(x));

            while (abs(fms::ulp(x_, x)) >= ulp) {
                ensure (iter-- > 0);
                x = x_;
                x_ = newton(x, f(x), df(x));
            }
            
            return x_;
        }

        template<class X, class Y>
        inline X secant_solve(X x0, X x1, const std::function<Y(X)>& f, 
            int iter = 100, long long ulp = 10)
        {
            Y y0 = f(x0);
            Y y1 = f(x1);
            X x_ = secant(x0, y0, x1, y1);

            while (abs(fms::ulp(x_, x1)) >= ulp) {
                ensure (iter-- > 0);
                x0 = x1;
                y0 = y1;
                x1 = x_
                y1 = f(x_);
                x_ = secant(x0, y0, x1, y1);
            }
            
            return x_;
        }
    } // root1d
} // fms
