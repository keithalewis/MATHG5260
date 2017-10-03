// black.h - Black forward option value.
#pragma once
#include "prob.h"

/*
The forward is F = f exp(-sigma^2 t/2 + sigma B_t)
where B_t is Brownian motion at time t.

A put option with strike k has forward value 
E max{k - F, 0}.

If N is normal, then E exp(N) = exp(E(N) + Var(N)/2)).
Also E f exp(N) g(N) = E f exp(N) E g(N + Var(N)).

So E max{k - F, 0}
 = E (k - F) 1(F <= k)
 = k P(F <= k) - E F 1(F <= k)
 = k P(F <= k) - E F P(F exp(sigma^2 t) <= k),

 N = -sigma^2 t/2 + sigma B_t.
 g(N) = 1(f exp(N) <= k
 g(N + Var N) = 1(f exp(N + sigma^2 t) <= k) = 1(F exp(sigma^2 t) <= k).

 Let s = sigma sqrt(t)
 Z = B_t/sqrt(t) ~ N(0,1)

 F <= k
 f exp(-s^2/2 + s Z) <= k
 -s^2/2 + s Z <= log(k/f)
 Z <= (s^2/2 + log(k/f))/s = z

 F exp(s^2) <= k
 Z <= (-s^2/2 + log(k/f)/s = z^*

 E max{k - F, 0} = k N(z) - f N(z^*).

*/

namespace fms {

    namespace black {

        //!!! Find and fix the bug in this code.
        template<class F, class S, class K, class T>
        inline auto put(F f, S sigma, K k, T t)
        {
            auto s = sigma*std::sqrt(t);
            auto z = (s*s/2 + log(k/f))/s;
            auto z_ = z - s*s;
            auto N = fms::prob::normal::cdf(z);
            auto N_ = fms::prob::normal::cdf(z_);
            
            return k * N - f * N_;
        }
    
    } // namespace black

} // namespace fms
