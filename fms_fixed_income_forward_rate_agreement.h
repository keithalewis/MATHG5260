// fms_fixed_income_forward_rate_agreement.h - Forward rate agreements
#pragma once

// fixed_income_forward_rate_agreement.h - cash deposit
#pragma once
#include "fms_fixed_income.h"
#include "fms_date.h"

namespace fms {
	namespace fixed_income {

		// E.g. cash_deposit(days{2}, months{3});
		template<class U = double, class C = double>
		struct forward_rate_agreement : public instrument<U, C> {
			::date::months tenor;
			C rate;
			forward_rate_agreement(::date::months t)
				: instrument<U, C>(2), tenor(t)
			{ }
			// Fix instrument times and cash flows
			// !!! fra's have two cash flows: -1 at effective, 1 + rate*dcf at effective + tenor.
			template<class DCB, class ROLL>
			forward_rate_agreement& fix(::date::year_month_day effective, C rate)
			{
				this->rate = rate;
/*
				auto val = ::date::sys_days(valuation);

				auto set = fms::date::adjust<ROLL>(::date::year_month_day(val + settlement));
				auto dset = ::date::sys_days(set) - val;
				U u0 = std::chrono::duration<U, ::date::years::period>(dset).count();
				time(0) = u0;
				cash(0) = -1;

				auto mat = fms::date::adjust<ROLL>(set + maturity);
				auto dmat = ::date::sys_days(mat) - val;
				U u1 = std::chrono::duration<U, ::date::years::period>(dmat).count();
				time(1) = u1;
				cash(1) = 1 + rate*fms::date::dcf<DCB>(set, mat);
*/
				return *this;
			}

		};
#ifdef _DEBUG
		inline void test_fms_fixed_income_forward_rate_agreement()
		{
			/*!!!Add suitable test.
			forward_rate_agreement<> cd(::date::days{ 2 }, ::date::months{ 3 });
			ensure(cd.size() == 2);
			::date::year_month_day date(::date::year{ 2017 }, ::date::month{ 10 }, ::date::day{ 26 });
			cd.fix<fms::date::actual_360, fms::date::modified_following>(date, 0.01);

			ensure(cd.time(0) == 0.0054758140139770156);
			ensure(cd.time(1) == 0.25736325865691972);
			ensure(cd.cash(0) == -1);
			ensure(fabs(cd.cash(1) - (1 + cd.rate*0.25)) < 1e-4);
			*/
		}
#endif // _DEBUG

	} // fixed_income
} // fms