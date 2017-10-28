// fixed_income_cash_deposit.h - cash deposit
#pragma once
#include "fms_fixed_income.h"
#include "fms_date.h"

namespace fms {
namespace fixed_income {

    template<class U = double, class C = double>
    struct cash_deposit : public instrument<U,C> {
        ::date::days settlement;
        ::date::months tenor;
        date::DAY_COUNT_BASIS dcb;
        date::BUSINESS_DAY_ROLL roll;
        const date::calendar& holiday;
        C rate;
        cash_deposit(::date::days settlement, ::date::months tenor, 
            date::DAY_COUNT_BASIS dcb = date::DCB_ACTUAL_365,
            date::BUSINESS_DAY_ROLL roll = date::ROLL_MODIFIED_FOLLOWING, 
            const date::calendar& holiday = date::holiday_none)
            : instrument<U,C>(2), settlement(settlement), tenor(tenor), dcb(dcb), roll(roll), holiday(holiday)
        { }
        // Fix instrument times and cash flows
        cash_deposit& fix(::date::year_month_day valuation, C r) override
        {
            rate = r;

            auto val = ::date::sys_days(valuation);

            auto set = fms::date::business_day_adjust(roll, ::date::year_month_day(val + settlement), holiday);
            auto dset = ::date::sys_days(set) - val;
            U u0 = std::chrono::duration<U,::date::years::period>(dset).count();
            time(0) = u0;
            cash(0) = -1;

            auto mat = fms::date::business_day_adjust(roll, set + tenor, holiday);
            auto dmat = ::date::sys_days(mat) - val;
            U u1 = std::chrono::duration<U,::date::years::period>(dmat).count();
            time(1) = u1;
            cash(1) = 1 + rate*fms::date::day_count_fraction(dcb, set, mat);
     
            return *this;
        }
    };
#ifdef _DEBUG
    inline void test_fms_fixed_income_cash_deposit()
    {
        using ::date::year_month_day;
        using ::date::year;
        using ::date::month;
        using ::date::day;

        cash_deposit<> cd(::date::days{2}, ::date::months{3}, date::DCB_ACTUAL_365, date::ROLL_FOLLOWING_BUSINESS);
        ensure (cd.size() == 2);

        // ymd + 2 is a Saturday
        year_month_day ymd(year{2017}, month{10}, day{26});
        cd.fix(ymd, 0.01);

        // ymd + 2 days is a Saturday
        auto set = year_month_day(year{2017}, month{10}, day{30});
        // set + 2 months is a Tuesday
        auto mat = year_month_day(year{2018}, month{1}, day{30});
        auto dcf = 92/365.;

        // Indepenedent "by hand" calculation.
        auto dset = ::date::sys_days(set) - ::date::sys_days(ymd);
        auto dmat = ::date::sys_days(mat) - ::date::sys_days(ymd);
        auto u0 = std::chrono::duration<double,::date::years::period>(dset);
        auto u1 = std::chrono::duration<double,::date::years::period>(dmat);

        ensure (cd.time(0) == u0.count());
        ensure (cd.time(1) == u1.count());
        ensure (cd.cash(0) == -1);
        ensure (cd.cash(1) == 1 + cd.rate*dcf);
    }
#endif // _DEBUG

} // fixed_income
} // fms