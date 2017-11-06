// fms_fixed_income_forward_rate_agreement.h - Forward Rate Agreements
// An fra has two cash flows: -1 at the effective date and 1 + rate*dcf at
// the effective date plus tenor, adjusted to a business day.
#pragma once

namespace fms {
namespace fixed_income {

    template<class U = double, class C = double>
    struct forward_rate_agreement : public instrument<U,C> {
        ::date::year_month_day effective;
        ::date::months tenor;
        date::DAY_COUNT_BASIS dcb;
        date::BUSINESS_DAY_ROLL roll;
        const date::calendar& holiday;
        C rate;
        forward_rate_agreement(::date::year_month_day effective, ::date::months tenor, 
            date::DAY_COUNT_BASIS dcb = date::DCB_ACTUAL_365,
            date::BUSINESS_DAY_ROLL roll = date::ROLL_MODIFIED_FOLLOWING, 
            const date::calendar& holiday = date::holiday_none)
            : instrument<U,C>(2), effective(effective), tenor(tenor), dcb(dcb), roll(roll), holiday(holiday)
        { }
        // Fix instrument times and cash flows
        forward_rate_agreement& fix(::date::year_month_day valuation, C r) override
        {
            rate = r;

            auto val = ::date::sys_days(valuation);

            auto eff = fms::date::business_day_adjust(roll, effective, holiday);
            auto deff = ::date::sys_days(eff) - val;
            U u0 = std::chrono::duration<U,::date::years::period>(deff).count();
            time(0) = u0;
            cash(0) = -1;

            auto mat = fms::date::business_day_adjust(roll, eff + tenor, holiday);
            auto dmat = ::date::sys_days(mat) - val;
            U u1 = std::chrono::duration<U,::date::years::period>(dmat).count();
            time(1) = u1;
            cash(1) = 1 + rate*fms::date::day_count_fraction(dcb, eff, mat);
     
            return *this;
        }
    };

} // fixed_income
} // fms