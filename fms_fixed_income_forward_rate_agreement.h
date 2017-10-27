// fms_fixed_income_forward_rate_agreement.h - Forward Rate Agreements
// An fra has two cash flows: -1 at the effective date and 1 + rate*dcf at
// the effective date plus tenor, adjusted to a business day.
#pragma once

namespace fms {
namespace fixed_income {

    template<class U = double, class C = double>
    struct forward_rate_agreement : public instrument<U,C> {
        ::date::months tenor;
        date::DAY_COUNT_BASIS dcb;
        date::BUSINESS_DAY_ROLL roll;
        C rate;
        forward_rate_agreement(::date::months tenor, date::DAY_COUNT_BASIS dcb, date::BUSINESS_DAY_ROLL roll)
            : instrument<U,C>(2), tenor(tenor), dcb(dcb), roll(roll)
        { }
        // Fix instrument times and cash flows
        forward_rate_agreement& fix(::date::year_month_day effective, C r) override
        {
            /*!!!Implement the times and cash flows for a fra
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
            */    
            
            return *this;
        }
    };
#ifdef _DEBUG
    /*
    inline void test_fms_fixed_income_forward_rate_agreement()
    {
        forward_rate_agreement<> cd(::date::days{2}, ::date::months{3}, fms::date::DCB_ACTUAL_360, fms::date::ROLL_MODIFIED_FOLLOWING);
        ensure (cd.size() == 2);
        ::date::year_month_day date(::date::year{2017}, ::date::month{10}, ::date::day{26});
        cd.fix(date, 0.01);

        ensure (cd.time(0) == 0.0054758140139770156);
        ensure (cd.time(1) == 0.25736325865691972);
        ensure (cd.cash(0) == -1);
        ensure (fabs(cd.cash(1) - (1 + cd.rate*0.25)) < 1e-4);
    }
    */
#endif // _DEBUG

} // fixed_income
} // fms