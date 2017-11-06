// fms_fixed_income_swap.h - fixed/floating rate swap
#pragma once
namespace fms {
namespace fixed_income {
    // The floating leg of a swap can be modeled with two fixed cash flows:
    // +1 at the effective date and -1 at the termination date.
    template<class U = double, class C = double>
    struct swap : public instrument<U,C> {
        ::date::days settlement;
        ::date::years tenor;
        date::PAYMENT_FREQUENCY freq; // of fixed leg
        date::DAY_COUNT_BASIS dcb; // of fixed leg
        date::BUSINESS_DAY_ROLL roll;
        const date::calendar& holiday;
        C rate;
        swap(::date::days settlement, ::date::years tenor, 
            date::PAYMENT_FREQUENCY freq = date::FREQ_SEMIANNUALLY,
            date::DAY_COUNT_BASIS dcb = date::DCB_ACTUAL_365,
            date::BUSINESS_DAY_ROLL roll = date::ROLL_MODIFIED_FOLLOWING, 
            const date::calendar& holiday = date::holiday_none)
            : instrument<U,C>(1 + tenor.count()*freq), settlement(settlement), tenor(tenor), freq(freq), dcb(dcb), roll(roll), holiday(holiday)
        { }
        // Fix instrument times and cash flows
        swap& fix(::date::year_month_day valuation, C r) override
        {
            rate = r;

            auto val = ::date::sys_days(valuation);

            auto set = fms::date::business_day_adjust(roll, ::date::year_month_day(val + settlement), holiday);
            auto dset = ::date::sys_days(set) - val;
            U u0 = std::chrono::duration<U,::date::years::period>(dset).count();
            time(0) = u0;
            cash(0) = -1;

            auto _u = set;
            for (size_t i = 1; i < size(); ++i) {
                auto ui = fms::date::business_day_adjust(roll, set + ::date::months{i*12/freq}, holiday);
                auto dui = ::date::sys_days(ui) - val;
                time(i) = std::chrono::duration<U,::date::years::period>(dui).count();
                cash(i) = rate*fms::date::day_count_fraction(dcb, _u, ui);
                _u = ui;
            }
            cash(size() - 1) += 1;
     
            return *this;
        }
    };
#ifdef _DEBUG
    inline void test_fms_fixed_income_swap()
    {
    }
#endif // _DEBUG

} // fixed_income
} // fms
