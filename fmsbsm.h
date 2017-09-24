// fmsbsm.h - Black-Sholes/Merton
// Formulas for pricing options using the Black-Scholes/Merton model.
#pragma once

/*
value = expected discounted cash flows

Assume interest rates are 0.

For a put option at k, on F = f exp(-sigma^2t/2 + sigma B_t)
where B_t is Brownian motion at time t.

Option value = E max{k - F, 0}.

If N is normal, then E exp(N) = exp(E(N) + Var(N)/2)).
Also E exp(N) f(N) = E exp(N) E f(N + Var(N)).

So E max{k - F, 0}
 = E (k - F) 1(F <= k)
 = k P(F <= k) - E F 1(F <= k)
 = k P(F <= k) - E F P*(F <= k), where P* = F/f P  and f = E F.

In the Black-Scholes/Merton model F = f exp(-s^2/2 + sZ) where
s = sigma sqrt(t) and Z is standard normal.

Note E exp(sZ) = exp(E(sZ) + Var(sZ)/2) = exp(s^2/2) so E F = f. 

P*(F <= k) = E exp(-s^2/2 + sZ) 1(F <= k) =
           = E exp(-s^2/2 + sZ) 1(F <= k) =

*/

namespace fms {

    namespace bsm {

        template<class F, class S, class K, class T>
        inline auto put(F f, S s, K k, T t)
        {

        }
    
    } // namespace bsm

} // namespace fms
