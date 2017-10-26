// fms_date.h - Date routines based on https://github.com/HowardHinnant/date
// The class year_month_day has members year, month, day.
// The class sys_days is a count of days from an epoch/time_point
#pragma once
#include "../date/include/date/date.h"

namespace fms {
namespace date {

    // day count basis
    class _30_360;
    class actual_360;
    class actual_365;
    //...

    // Day count fraction from t0 to t1 using day count basis.
    template<class DCB>
    inline auto dcf(::date::year_month_day t0, ::date::year_month_day t1);
    
    // 30 days in a month, 360 days in a year. 
    template<>
    inline auto dcf<_30_360>(::date::year_month_day t0, ::date::year_month_day t1)
    {

        auto y = t1.year() - t0.year();
        auto m = t1.month() - t0.month();
        auto d = t1.day() - t0.day();

        return y.count() + m.count()/30. + d.count()/360.;
    }
    // Number of days divided by 360.
    template<>
    inline auto dcf<actual_360>(::date::year_month_day t0, ::date::year_month_day t1)
    {
        return (::date::sys_days(t1) - ::date::sys_days(t0)).count()/360.;
    }
    // Number of days divided by 365.
    template<>
    inline auto dcf<actual_365>(::date::year_month_day t0, ::date::year_month_day t1)
    {
        return (::date::sys_days(t1) - ::date::sys_days(t0)).count()/365.;
    }

    inline bool is_business_day(::date::year_month_day t/*, calendar c*/)
    {
        auto dow = ::date::weekday(t);

        return dow == ::date::sat || dow == ::date::sun;
    }

    // Relatively expensive operation not implmented by date library.
    inline ::date::year_month_day increment(::date::year_month_day t, ::date::days d)
    {
        return ::date::year_month_day(::date::sys_days(t) + d);
    }

    // business day conventions
    class following_business;
    class modified_following;

    template<class ROLL>
    inline ::date::year_month_day adjust(::date::year_month_day t/*, calendar c*/);

    // Move forward to next business day.
    template<>
    inline ::date::year_month_day adjust<following_business>(::date::year_month_day t/*, calendar c*/)
    {
        while (!is_business_day(t)/*, c*/)
            t = increment(t, ::date::days{1});

        return t;
    }
    // Move forward to next business day unless it is in the next month.
    template<>
    inline ::date::year_month_day adjust<modified_following>(::date::year_month_day t/*, calendar c*/)
    {
        auto m = t.month();
        while (!is_business_day(t/*, c*/) && m == t.month())
            t = increment(t, ::date::days{1});
        while (!is_business_day(t/*, c*/))
            t = increment(t, ::date::days{-1});

        return t;
    }

    // weekday_indexed
} // date
} // fms