// fms_date.h - Date routines based on https://github.com/HowardHinnant/date
#pragma once
#include "../date/include/date/date.h"

namespace fms {
namespace date {
    // day count fractions
    class actual_360 {};
    class actual_365 {};
    class _30_360 {};
/*
    template<class Ratio, class DCB>
    double day_count_fraction(const std::chrono::duration<int,Ratio>& interval, DCB);

    template<std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>
    inline double day_count_fraction(const days& interval, actual_360)
    {
        return interval.count()/360;
    }
*/
    // business day conventions
    class modified;
    class modified_following;

    // weekday_indexed
} // date
} // fms