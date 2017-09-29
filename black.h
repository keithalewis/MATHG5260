// fmsbsm.h - Black-Sholes/Merton
// Formulas for pricing options using the Black-Scholes/Merton model.
#pragma once

/*
Fischer Black forward value model.

For a put option at k, on F = f exp(-sigma^2t/2 + sigma B_t)
where B_t is Brownian motion at time t.

Option forward value = E max{k - F, 0}.

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

        template<class F, class S, class K, class T>
        inline auto put(F f, S s, K k, T t)
        {
            return 0;
        }
    
    } // namespace black

} // namespace fms
