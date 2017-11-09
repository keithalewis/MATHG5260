// fms_bootstrap.h - Bootstrap a piecewise flat forward curve.
#pragma once
#include <limits>
#include "ensure.h"
#include "fms_root1d.h"
#include "fms_pwflat.h"
#include "fms_fixed_income.h"

namespace fms {
namespace pwflat {
	
	// Extrapolate curve to match price with present value.
	template<class T, class F>
	inline std::pair<T,F> bootstrap(F p, size_t m, const T* u, const F* c, size_t n, const T* t, const F* f, F _f = 0, bool use_newton = false)
	{
        // expiration must be past the end of the forward curve
        ensure (m > 0);
        ensure (n == 0 || u[m-1] > t[n-1]);
        
        if (use_newton)
            goto use_newton_solve;

        // end of curve 
        auto t_ = n == 0 ? 0 : t[n-1];
        // discount to end
        auto D_ = discount(t_, n, t, f);
        // last cash flow
        auto c_ = c[m - 1];
        // last cash flow time
        auto u_ = u[m - 1];

        //!!! New Code
        // If only one cash flow occurs past the end of the curve there is a closed form solution:
        // We have p = pv + c D e^{-f(u - t)}, where pv is the present value of all but the last
        // cash flow, c is the last cash flow, and u is the last cash flow time.
        if (m == 1 || u[m - 2] <= t_) {
            auto pv = present_value(m - 1, u, c, n, t, f);
            
            return std::make_pair(u_, log((p - pv)/(c_*D_))/(t_ - u_));
        }

        //!!! New Code
        // If exactly two cash flows and price is 0, then we know u[0] > t_ or else
        // the previous case would hold.
        // 0 = c0 D exp(-f(u0 - t)) + c1 D exp(-f(u1 -t)) so
        // f = - log(-c0/c1)/(u1 - u0).
        if (p == 0 && m == 2) {
            ensure (u[0] > t_);
            ensure (u[0] < u[1]);

            return std::make_pair(u_, log(-c[0]/c[1])/(u[0] - u[1]));
        }

    use_newton_solve:

        std::function<F(F)> pv = [p, m, u, c, n, t, f](F _f) {
			return -p + present_value(m, u, c, n, t, f, _f);
		};
		std::function<F(F)> dpv = [m, u, c, n, t, f](F _f) {
			return partial_duration(m, u, c, n, t, f, _f);
		};

		// Use fms::root1d::newton to solve for the extrapolated value.
		if (n > 0)
			_f = f[n-1];

        //???Check if a solution is possible.

		_f = root1d::newton_solve<F,F>(_f, pv, dpv);

		return std::make_pair(u_,_f);
	}

	template<class T, class F>
	inline std::pair<T,F> bootstrap(F p, const fixed_income::interface<T,F>& i, const pwflat::interface<T,F>& f, F _f = 0, bool use_newton = false)
    {
        return bootstrap(p, i.size(), i.time(), i.cash(), f.size(), f.time(), f.rate(), _f, use_newton);
    }
    /*
    template<class T, class F>
    curve<T,F> bootstrap("container of instruments")
    {
        curve<T,F> f;

        for (const auto& i : instruments) {
            f.push_back(bootstrap(i.price, i, f)));
        }

        return f;
    }
    */
#ifdef _DEBUG
#pragma warning(push)
#pragma warning(disable: 4189)
    //!!! Use the use_newton argument in bootstrap to test the 
    //!!! one and two cash flow closed form solutions.
    inline void fms_pwflat_bootstrap_test(void)
    {
        double t[] = {1};
        double f[] = {0.1};
        
        {
            //!!! test a cash deposit
            double u[] = {2};
            double c[] = {1 + 0.01*u[0]};
            double p = 1;
        }
        {
            //!!! test a forward rate agreement
            double u[] = {2, 3};
            double c[] = {-1, 1 + 0.01};
            double p = 0;
        }
    }
#pragma warning(pop)
#endif // _DEBUG
}
} // namespace fms
