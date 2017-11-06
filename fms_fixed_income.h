// fms_fixed_income.h - Fixed Income instruments
// The _valuation date_ is the date that prices, rates, coupons are quoted.
// Each instrument has a _settlement date_ when the first cash flow is exchanged.
// We assume all instruments have price zero and the cash flow on the settlement 
// date is the negative of the price.

#pragma once
#include <vector>
#include "../date/include/date/date.h"
#include "ensure.h"

namespace fms {
namespace fixed_income {

    // NVI instrument interface.
    template<class U = double, class C = double>
    class interface {
        size_t n;
        U* u;
        C* c;
    public:
        typedef U time_type;
        typedef C cash_type;
        interface(size_t n = 0, U* u = 0, C* c = 0)
            : n(n), u(u), c(c)
        { }
        interface(const interface&) = default;
        interface& operator=(const interface&) = default;
        virtual ~interface() {}
        size_t   size() const { return _size(); }
        const U* time() const { return _time(); }
        const C* cash() const { return _cash(); }
        
        // convenience functions
        bool operator==(const interface& i) const
        {
            return size() == i.size()
                && std::equal(time(), time() + size(), i.time())
                && std::equal(cash(), cash() + size(), i.cash());
        }
        bool operator!=(const interface& i) const
        {
            return !operator==(i);
        }
        const U& time(size_t i) const
        {
            return _time()[i];
        }
        const C& cash(size_t i) const
        {
            return _cash()[i];
        }
//    protected:
        U& time(size_t i)
        {
            return _time()[i];
        }
        C& cash(size_t i)
        {
            return _cash()[i];
        }
    private:
        virtual size_t   _size() const
        {
            return n;
        }
        virtual U* _time() const
        {
            return u;
        }
        virtual C* _cash() const
        {
            return c;
        }
    };

    // Generic fixed income instrument.
    template<class U = double, class C = double>
    class instrument : public interface<U,C> {
        std::vector<U> u_; // cash flow times in years
        std::vector<C> c_; // cash flows
    public:
        instrument()
        { }
        instrument(size_t n)
            : u_(n), c_(n)
        { }
        instrument(size_t n, const U* u, const C* c)
            : u_(u, u + n), c_(c, c + n)
        { }
        instrument(const std::vector<U>& u, const std::vector<C>& c)
            : u_(u), c_(c)
        {
            ensure (u_.size()  == c_.size());
        }
		instrument(const instrument&) = default;
		instrument& operator=(const instrument&) = default;
		~instrument()
        { }
		virtual instrument& fix(::date::year_month_day, C)
		{
            return *this;
        }
    private:
        size_t _size() const override
        {
            return u_.size();
        }
        U* _time() const override
        {
            return const_cast<U*>(&u_[0]);
        }
        C* _cash() const override
        {
            return const_cast<C*>(&c_[0]);
        }
    };
#ifdef _DEBUG
    inline void test_fms_fixed_income_instrument()
    {
        double u[] = {1,2};
        double c[] = {.1,.2};
        instrument<> i(2, u, c);
            
        instrument<> i2(i);
        ensure (i == i2);
        i = i2;
        ensure (i == i2);
        ensure (!(i != i2));

        ensure (i.size() == 2);
        ensure (i.time()[0] == u[0]);
        ensure (i.cash(1) == c[1]);
    }
#endif // _DEBUG

#if 0
    /*
    template<class D, class U = double, class C = double>
    class cash_deposit : public instrument<U,C> {
    public:
        template<class D> // duration
        cash_deposit(D duration, D settlement)
    };

    template<class U = double, class C = double, class D = double>
    struct forward_rate_agreement : public interface<U,C> {
        U u[2];
        C c[2];
        D effective;
        D termination;
        C rate;
        C price;
        forward_rate_agreement()
        { }
        forward_rate_agreement(U effective, U termination)
            : effective(effective), term(term)
        {
        }
        forward_rate_agreement(const forward_rate_agreement& fra)
        {
            this->u[0] = fra.u[0];
            this->u[1] = fra.u[1];
            this->c[0] = fra.c[0];
            this->c[1] = fra.c[1];
        }
        ~forward_rate_agreement()
        { }
        forward_rate_agreement& fix(C rate, C price = 0) 
        {
            this->rate = rate;
            this->price = price;
            u[0] = effective;
            u[1] = effective + term;
            c[0] = -1;
            c[1] = 1 + rate*(u[1] - u[0]);

            return *this;
        }
    private:
        size_t   _size() const { return 2; }
        const U* _time() const { return u; }
        const C* _cash() const { return c; }
    };

    /*
    enum day_count_basis { actual_360, actual_365, _30_360 };
    enum business_day_roll { following, preceding, modified_following };
    typedef double duration;
    enum calendar { nys };
    
    template<class U = double, class C = double>
    struct cash_deposit : public interface<U,C> {
        int settlement;
        C rate;
        duration term;
        day_count_basis dcb;
        business_day_roll roll;
        holiday_calendar calendar;
        U u[1];
        C c[1];

        // e.g. cash_deposit cd(day{2}, 0.01, months{3}, acutal_365, modified_following);
        cash_deposit(int settlement, C rate, duration term, day_count_basis dcb, business_day_roll roll)
            : settlement(settlement), rate(rate), term(term), dcb(dcb), roll(roll)
        { }

        template<D = year_month_day>
        cash_deposit& fix(const C& price, const C& rate, D valuation)
        {
            auto set = calendar(roll(valuation + settlement));
            auto mat = calendar(roll(set + term))
            u[0] = diff_year(roll(mat, valuation);
            c[0] = 1 + rate*dcf(mat - set, dcb);
        }
    private:
        size_t _size() const { return 1; }
        const U* _time() const { return u; }
        const C* _cash() const { return c; }
    };
    */

#endif // 0
} // fixed_income
} // fms
