// black.h - Black forward option value.
#pragma once
#include <algorithm>
#include "ensure.h"
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

 d/df E g(F) = E g'(F) F/f = E g'(F exp(s^2))
*/

namespace fms {

    namespace black {

        template<class F, class S, class K>
        inline auto value(F f, S s, K k)
        {
            ensure (f >= 0);
            ensure (s >= 0);
            ensure (k >= 0);

            if (f == 0 || s == 0)
                return k <= f ? k - f : 0;

            if (k == 0)
                return (F)0;

            auto z = s/2 + log(k/f)/s;
            auto z_ = z - s;
            auto N = fms::prob::normal::cdf(z);
            auto N_ = fms::prob::normal::cdf(z_);
            
            return k * N - f * N_;
        }
        // derivative of value with respect to forward
        template<class F, class S, class K>
        inline auto delta(F f, S s, K k)
        {
            ensure (f >= 0);
            ensure (s >= 0);
            ensure (k >= 0);

            if (f == 0 || s == 0)
                return 1*(k >= f);

            if (k == 0)
                return 0;

            auto z_ = -s/2 + log(k/f)/s;

            return - fms::prob::normal::cdf(z_);
        }
        
        // forward put value
        template<class F, class S, class K, class T>
        inline auto put(F f, S sigma, K k, T t)
        {
            return value(f, sigma*sqrt(t), k);
        }
        template<class F, class S, class K, class T>
        inline auto put_delta(F f, S sigma, K k, T t)
        {
            return delta(f, sigma*sqrt(t), k);
        }
        template<class F, class S, class K, class T>
        inline auto put_vega(F f, S sigma, K k, T t)
        {
            ensure (f > 0);
            ensure (sigma > 0);
            ensure (k > 0);
            ensure (t > 0);

            auto sqt = sqrt(t);
            auto s = sigma*sqt;
            auto z = s/2 + log(k/f)/s;
            auto n = fms::prob::normal::pdf(z);
            
            return f*n*sqt;
        }
        //!!! implement this and create the add-in
        //!!! BLACK.PUT.IMPLIED.VOLATILITY
        // volatility that matches put price
        template<class F, class P, class K, class T>
        inline auto put_implied_volatility(F f, P p, K k, T t)
        {
            // use lambdas for f and df in newton_solve

            return 0;
        }

        // forward call value using put-call parity
        // c - p = f - k
        template<class F, class S, class K, class T>
        inline auto call(F f, S sigma, K k, T t)
        {
            return put(f, sigma, k, t) + f - k;
        }
        template<class F, class S, class K, class T>
        inline auto call_delta(F f, S sigma, K k, T t)
        {
            return put_delta(f, sigma, k, t) + 1;
        }
    
    } // namespace black

} // namespace fms
