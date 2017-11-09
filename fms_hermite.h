// hermit.h - Hermite polynomials
// H_{n+1}(x) = x H_n(x)−n H_{n−1}(x), H_0(x) = 1, H_1(x) = x. 
#pragma once
namespace fms {

    //!!! Implement this
    template<class X>
    inline X Hermite(size_t n, X x)
    {
        return 0;
    }

#ifdef _DEBUG

    inline void fms_hermite_test(void)
    {
        //!!! Add tests for Hermite polynomials.
        //!!! for n = 0,1,..4
        //!!! and x = 0.2
    }

#endif // _DEBUG

} // fms
