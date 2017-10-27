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
        C rate;
        cash_deposit(::date::days s, ::date::months m, date::DAY_COUNT_BASIS dcb, date::BUSINESS_DAY_ROLL roll)
            : instrument<U,C>(2), settlement(s), tenor(m), dcb(dcb), roll(roll)
        { }
        // Fix instrument times and cash flows
        cash_deposit& fix(::date::year_month_day valuation, C r) override
        {
            rate = r;

            auto val = ::date::sys_days(valuation);

            auto set = fms::date::business_day_adjust(roll, ::date::year_month_day(val + settlement));
            auto dset = ::date::sys_days(set) - val;
            U u0 = std::chrono::duration<U,::date::years::period>(dset).count();
            time(0) = u0;
            cash(0) = -1;

            auto mat = fms::date::business_day_adjust(roll, set + tenor);
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
        cash_deposit<> cd(::date::days{2}, ::date::months{3}, fms::date::DCB_ACTUAL_360, fms::date::ROLL_MODIFIED_FOLLOWING);
        ensure (cd.size() == 2);
        ::date::year_month_day date(::date::year{2017}, ::date::month{10}, ::date::day{26});
        cd.fix(date, 0.01);

        ensure (cd.time(0) == 0.0054758140139770156);
        ensure (cd.time(1) == 0.25736325865691972);
        ensure (cd.cash(0) == -1);
        ensure (fabs(cd.cash(1) - (1 + cd.rate*0.25)) < 1e-4);
    }
#endif // _DEBUG

} // fixed_income
} // fms