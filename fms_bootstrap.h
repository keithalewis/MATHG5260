// fms_bootstrap.h - Bootstrap a piecewise flat forward curve.
#pragma once
#include <limits>
#include "fms_root1d.h"
#include "fms_pwflat.h"
#include "fms_fixed_income.h"

namespace fms {
namespace pwflat {
	
	// Extrapolate curve to match price with present value.
	template<class T, class F>
	inline F bootstrap(F p, size_t m, const T* u, const F* c, size_t n, const T* t, const F* f, F _f = 0)
	{
        // expiration must be past the end of the forward curve
        if (m == 0 || n != 0 && u[m-1] <= t[n-1])
            return std::numeric_limits<F>::quiet_NaN();

        // Zero coupon bond (m == 1)
        // p = c0 D(u0)
        // if n == 0 then Dn = 1
        // else Dn = D(t[n-1])
        // D(u0) = Dn*exp(-f*(u[0] - t[n-1])).
        // p = c0 Dn*exp(-f dt);
        // log(p/(c0 Dn)) = -f dt
        // f = -log(p/(c0 Dn))/dt
        if (m == 1) {
            auto Dn = n == 0 ? 1 : discount(t[n-1], n, t, f);
            auto tn = n == 0 ? 0 : t[n-1];
            auto dt = u[0] - tn;
         
            _f = -log(p/(c[0]*Dn))/dt;

            return _f;
        }

        // Forward rate agreement (m == 2 and p = 0)
        // 0 = c0 D(u0) + c1 D(u1)
        // if n == 0 then 0 = c0 exp(-f u0) + c1 exp (-f u1)
        // c0 exp(-f u0) = -c1 exp (-f u1)
        // so exp(-f u0)/exp(-f u1) = - c1/c0.
        //    exp(f(u1 - u0)) = - c1/c0
        //    f = log(-c1/c0)/(u1 - u0)
        // if u0 < t[n-1] 
        // then D(u0) = discount(u0)
        // and D(u1) = Dn exp(-f(u1 - t[n-1])
        // so 0 = c0 Du0 + c1 Dn exp(-f(u1 - tn))
        //    exp(-f dt) = -c0 Du0/(c1 Dn)
        //    f = log(-c0 Du0/(c1 Dn)/dt
        // if u0 > t[n-1] 
        // then D(u0) = Dn exp(-f(u0 - t[n-1])
        // and D(u1) = Dn exp(-f(u1 - t[n-1]))
        if (m == 2 && p == 0) {
            auto c10 = c[1]/c[0];
            if (c10 >= 0)
                return std::numeric_limits<F>::quiet_NaN();
            if (n == 0)
                _f = log(-c10)/(u[1] - u[0]);
            else {
                if (u[0] < t[n-1]) {
                    auto Du0 = discount(u[0], n, t, f);
                    auto Dn = discount(t[n-1], n, t, f);
                    _f = log(-c[0]*Du0/(c[1]*Dn))/(u[1] - t[n-1]);
                }
                else {
                    //... 
                }
            }
            
            return _f;
        }

		std::function<double(double)> pv = [p, m, u, c, n, t, f](double _f) {
			return -p + present_value(m, u, c, n, t, f, _f);
		};
		std::function<double(double)> dpv = [m, u, c, n, t, f](double _f) {
			return duration_extrapolated(m, u, c, n, t, f, _f);
		};

		// Use fms::root1d::newton to solve for the extrapolated value.
		if (n > 0)
			_f = f[n-1];

		_f = root1d::newton_solve<F,F>(_f, pv, dpv);

		return _f;
	}

    /*
    template<class T, class F>
    curve<T,F> bootstrap("container of instruments")
    {
        curve<T,F> f;

        for (const auto& i : instruments) {
            T t = i.maturity();
            f.push_back(make_pair(t, bootstrap(i.price, i, f)));
        }

        return f;
    }
    */
}
} // namespace fms
