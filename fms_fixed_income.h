// fms_fixed_income.h - Fixed Income instruments
#pragma once
#include <vector>
#include "ensure.h"
#include "fms_date.h"

namespace fms {
namespace fixed_income {

    // instrument interface
    template<class U = double, class C = double>
    class interface {
    public:
        typedef U time_type;
        typedef C cash_type;
        virtual ~interface() {}
        size_t   size() const { return _size(); }
        const U* time() const { return _time(); }
        const C* cash() const { return _cash(); }
    private:
        virtual size_t   _size() const = 0;
        virtual const U* _time() const = 0;
        virtual const C* _cash() const = 0;
    };

    template<class U = double, class C = double>
    class instrument : public interface<U,C> {
        std::vector<U> u_;
        std::vector<C> c_;
    public:
        instrument()
        { }
        instrument(size_t n, const U* u, const C* c)
            : u_(u, u + n), c_(c, c + n)
        { }
        instrument(const std::vector<U>& u, const std::vector<C>& c)
            : u_(u), c_(c)
        {
            ensure (u_.size()  == c_.size());
        }
        ~instrument()
        { }
    private:
        size_t _size() const
        {
            return u_.size();
        }
        const U* _time() const
        {
            return u_.data();
        }
        const C* _cash() const
        {
            return c_.data();
        }
    };

    template<class U = double, class C = double>
    struct cash_deposit : public interface<U,C> {
        U u;
        C c;
        U term;
        C price;

        cash_deposit(U term)
            : term(term)
        {
        }
        cash_deposit& fix(C rate, C price = 1)
        {
            this->price = price;
            u = term;
            c = 1 + rate*term;

            return *this;
        }
    private:
        size_t _size() const { return 2; }
        const U* _time() const { return &u; }
        const C* _cash() const { return &c; }
    };

    template<class U = double, class C = double>
    struct forward_rate_agreement : public interface<U,C> {
        std::vector<U> u;
        std::vector<C> c;
        U effective;
        U term;
        C price;
        forward_rate_agreement(U effective, U term)
            : effective(effective), term(term), u(2), c(2)
        {
        }
        forward_rate_agreement& fix(C rate, C price = 0) 
        {
            this->price = price;
            u[0] = effective;
            u[1] = effective + term;
            c[0] = -1;
            c[1] = 1 + rate*(u[1] - u[0]);

            return *this;
        }
    private:
        size_t _size() const { return 2; }
        const U* _time() const { return u.data(); }
        const C* _cash() const { return c.data(); }
    };

#if 0
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
