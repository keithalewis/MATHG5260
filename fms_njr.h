// fms_njr.h - Normal Jarrow-Rudd model
#pragma once
#include <vector>
#include "fms_bell.h"
#include "fms_hermite.h"
#include "fms_prob.h"

namespace fms {
    namespace prob {
        // Apply esscher transform to cumulants.
        // kappa_[m] = sum_{k >= 0} kappa[m + k] s^k/k!
        template<class X>
        void esscher(X s, size_t n, const X* kappa, X* kappa_)
        {
            for (size_t m = 0; m < n; ++m) {
                kappa_[m] = 0;
                X s_ = 1;
                size_t k_ = 1;
                for (size_t k = 0; k + m < n; ++k) {
                    kappa_[m] += kappa[m + k]*s_/k_;
                    s_ *= s;
                    k_ *= k_ + 1;
                }
            }
        }

        template<class X = double>
        struct njr {
            // psi(x) = phi(x) sum_{n>=0} B_n(kappa...) H_n(x)
            static X pdf(X x, size_t n, const X* kappa)
            {
                X phi = normal::pdf(x);

                size_t m_ = 1;
                X BH = 0;
                for (size_t m = 0; m < n; ++m) {
                    BH += Bell(m, kappa) * Hermite(m, x)/m_;
                    m_ *= m + 1;
                }

                return phi;
            }
            // Psi(x) = Phi(x) - phi(x) sum_{n>=1} B_n(kappa...) H_{n-1}(x)
            static X cdf(X x, size_t n, const X* kappa)
            {
                X Phi = normal::cdf(x);
                X phi = normal::pdf(x);

                X BH = 0;
                size_t k_ = 1;
                for (size_t k = 1; k <= n; k++) {
                    k_ *= k; // k!
                    BH += Bell(k, kappa)*Hermite(k - 1, x)/k_;
                }

                return Phi - phi*BH;
            }
        };
    } // prob

    namespace njr {

        // cumulant from cumulants kappa(s) = sum_{k>=0} kappa_k s^k/k!
        template<class X>
        inline X kappas(X s, size_t n, const X* kappa)
        {
            X ks = 0;
            
            X s_ = 1;
            size_t k_ = 1; // k!
            for (size_t k = 0; k < n; ++k) {
                ks += kappa[k]*s_/k_;
                s_ *= s;
                k_ *= k + 1;
            }

            return ks;
        }

        // put option value given perturbation of cumulants
        template<class F = double, class S = double, class K = double>
        inline auto value(F f, S s, K k, size_t n, const F* kappa)
        {
            ensure (f >= 0);
            ensure (s >= 0);
            ensure (k >= 0);

            if (f == 0 || s == 0)
                return k <= f ? k - f : 0;

            if (k == 0)
                return (F)0;

            auto z = (kappas(s, n, kappa) + log(k/f))/s;
            auto N = prob::njr<F>::cdf(z, n, kappa);

            std::vector<F> kappa_(n);
            prob::esscher(s, n, kappa, kappa_.data());
            auto N_ = prob::njr<F>::cdf(z, n, kappa_.data());
            
            return k * N - f * N_;
        }
        template<class F = double, class S = double, class K = double, class T = double>
        inline auto put(F f, S sigma, K k, double t, size_t n, const F* kappa)
        {
            return value<F,S,K>(f, sigma*sqrt(t), k, n, kappa);
        }
                // forward call value using put-call parity
        // c - p = f - k
        template<class F, class S, class K, class T>
        inline auto call(F f, S sigma, K k, T t, size_t n, const F* kappa)
        {
            return put(f, sigma, k, t, n, kappa) + f - k;
        }

    } // njr
} // fms