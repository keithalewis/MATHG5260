// ulp.h - units in the last place
// Copyright (c) 2006-2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cstdint>

namespace fms {
    template<class T>
    struct ulp_traits { };

    template<>
    struct ulp_traits<float> {
	    typedef int32_t integer;
	    static const integer x80_ = 0x80000000L;
    };
    template<>
    struct ulp_traits<double> {
	    typedef int64_t integer;
	    static const integer x80_ = 0x8000000000000000LL;
    };

    template<class T>
    inline typename ulp_traits<T>::integer ulp(T x, T y)
    {
	    union { T d; ulp_traits<T>::integer l; } ix, iy;

	    ix.d = x;
	    iy.d = y;

	    if (ix.l < 0)
		    ix.l = ulp_traits<T>::x80_ - ix.l;

	    if (iy.l < 0)
		    iy.l = ulp_traits<T>::x80_ - iy.l;

	    return ix.l - iy.l;
    }
}
