// fms_nsr.h - Normal short rate model.
#pragma once
#include <cmath>

namespace fms {
namespace nsr {

	// D_t(u) is the price at time t of a zero maturing at u.
	// From http://kalx.net/columbia/nsr.html we have for constant sigma
	// -log D_t(u) = \int_t^u f(s) ds + sigma^2 ut(u - t)/2 + sigma (u - t) B_t.
	// Let Dt = D_0(t) and Du = D_0(u).
	// -log D_0(t) = \int_0^t f(s) ds + sigma t B_t.
	// -log D_0(u) = \int_0^t f(u) ds + sigma u B_u.
    // \int_t^u f(s) ds = \int_0^u - \int_^t ...
	// -log D_t(u) = -log Du - sigma u B_u + log Dt + sigma t B_t 
    //              + sigma^2 ut(u - t)/2 + sigma (u - t) B_t.
	// -log D_t(u) = -log Du - sigma u B_u + log Dt +  
    //              + sigma^2 ut(u - t)/2 + sigma (u) B_t.
	// -log D_t(u) = -log Du + log Dt +  
    //              + sigma^2 ut(u - t)/2 + sigma u (B_t - B_u).
    
	// The expected value of log D_t(u)
	template<class T, class X>
	inline X E_logD(X sigma, X Dt, X Du, T t, T u)
	{
        // ensure (t < u);
		return log(Du) - log(Dt) - sigma*sigma*u*t*(u - t)/2;
	}
	// The variance of log D_t(u)
	template<class T, class X>
	inline X Var_logD(X sigma, X Dt, X Du, T t, T u)
	{
        // ensure (t < u);
		return sigma*sigma*u*u*(u - t);
    }
} // namespace nsr
} // namespace fms