// fms_date.h - Date routines based on https://github.com/HowardHinnant/date
// See also: https://fmsdatetime.codeplex.com/SourceControl/latest#trunk/datetime.h
// The class year_month_day has members year, month, day.
// The class sys_days is a count of days from an epoch/time_point
#pragma once
#include <functional>
#include "../date/include/date/date.h"

namespace fms {
namespace date {

    // day count basis
    class _30_360;
    class actual_360;
    class actual_365;
    //...
    enum DAY_COUNT_BASIS {
        DCB_30_360,
        DCB_ACTUAL_360,
        DCB_ACTUAL_365,
    };

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
    inline double day_count_fraction(DAY_COUNT_BASIS dcb, ::date::year_month_day t0, ::date::year_month_day t1)
    {
        switch (dcb) {
        case DCB_30_360:
            return dcf<_30_360>(t0, t1);
        case DCB_ACTUAL_360:
            return dcf<actual_360>(t0, t1);
        case DCB_ACTUAL_365:
            return dcf<actual_365>(t0, t1);
        }

        return std::numeric_limits<double>::quiet_NaN();
    }

    using calendar = std::function<bool(::date::year_month_day)>;
    static calendar holiday_none = [](::date::year_month_day) { return false; };

    inline bool is_business_day(::date::year_month_day ymd, const calendar& holiday)
    {
        auto dow = ::date::weekday(ymd);

        return dow != ::date::sat && dow != ::date::sun && !holiday(ymd);
    }

    // Relatively expensive operation not implmented by date library.
    inline ::date::year_month_day increment(::date::year_month_day ymd, ::date::days d)
    {
        return ::date::year_month_day(::date::sys_days(ymd) + d);
    }

    // business day conventions
    class following_business;
    class modified_following;
    // ...
	enum BUSINESS_DAY_ROLL {
		ROLL_FOLLOWING_BUSINESS,
		ROLL_MODIFIED_FOLLOWING,
	};

    template<class ROLL>
    inline ::date::year_month_day adjust(::date::year_month_day ymd, const calendar& holiday);

    // Move forward to next business day.
    template<>
    inline ::date::year_month_day adjust<following_business>(::date::year_month_day ymd, const calendar& holiday)
    {
        while (!is_business_day(ymd, holiday))
            ymd = increment(ymd, ::date::days{1});

        return ymd;
    }
    // Move forward to next business day unless it is in the next month.
    template<>
    inline ::date::year_month_day adjust<modified_following>(::date::year_month_day ymd, const calendar& holiday)
    {
        auto m = ymd.month();
        while (!is_business_day(ymd, holiday) && m == ymd.month())
            ymd = increment(ymd, ::date::days{1});
        while (!is_business_day(ymd, holiday))
            ymd = increment(ymd, ::date::days{-1});

        return ymd;
    }
    inline ::date::year_month_day business_day_adjust(BUSINESS_DAY_ROLL roll, ::date::year_month_day ymd, const calendar& holiday)
    {
        switch (roll) {
        case ROLL_FOLLOWING_BUSINESS:
            return adjust<following_business>(ymd, holiday);
        case ROLL_MODIFIED_FOLLOWING:
            return adjust<modified_following>(ymd, holiday);
        }
        
        return ::date::year_month_day{};
    }

    // Convert Excel Julian date to year_month_day
    inline ::date::year_month_day excel_date(double date)
    {
        static constexpr ::date::year_month_day excel_epoch = ::date::jan/0/1900;
        static constexpr ::date::sys_days epoch(excel_epoch);
        
        return ::date::year_month_day(epoch + ::date::days(static_cast<int>(date)));
    }
    // Convert year_month_day to Excel Julian date
    inline double excel_date(::date::year_month_day date)
    {
        static constexpr ::date::year_month_day excel_epoch = ::date::jan/0/1900;
        static constexpr ::date::sys_days epoch(excel_epoch);
        
        return std::chrono::duration<double,::date::days::period>(::date::sys_days(date) - epoch).count();
    }

    // weekday_indexed
} // date
} // fms