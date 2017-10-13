// fms_bootstrap.h - Bootstrap a piecewise flat forward curve.
#pragma once
#include <limits>
#include "fms_root1d.h"
#include "fms_pwflat.h"

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
        //!!! if (m == 1) {
        //!!!    return closed form solution for f
        //!!! }

        // Forward rate agreement (m == 2 and p = 0)
        // 0 = c0 D(u0) + c1 D(u1)
        // if n == 0 then 0 = c0 exp(-f u0) + c1 exp (-f u1)
        // if u0 < t[n-1] then D(u0) = discount(u0)
        //   and D(u1) = ???
        // else D(u0) = discount(t[n-1])*exp(-f(u0 - t[n-1]))
        //   and D(u1) = ???
        //!!! if (m == 2 && p == 0) {
        //!!!    return closed form solution for f
        //!!! }

		auto pv = [p, m, u, c, n, t, f](double _f) {
			return -p + present_value(m, u, c, n, t, f, _f);
		};
		auto dpv = [m, u, c, n, t, f](double _f) {
			return duration_extrapolated(m, u, c, n, t, f, _f);
		};

		// Use fms::root1d::newton to solve for the extrapolated value.
		if (n > 0)
			_f = f[n-1];

		_f = root1d::newton<F,F>(_f, pv, dpv);

		return _f;
	}

}
} // namespace fms
