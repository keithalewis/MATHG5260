// xll_date.cpp - date routines
#include "fms_date.h"
#include "G5260.h"

using namespace std::chrono;
using namespace date;
using namespace fms::date;
using namespace xll;

static constexpr year_month_day excel_epoch = jan/0/1900;

static AddIn xai_date(
    Function(XLL_DOUBLE, L"?xll_date", L"XLL.DATE")
    .Arg(XLL_SHORT, L"year", L"is the year of the date")
    .Arg(XLL_USHORT, L"month", L"is the month of the date")
    .Arg(XLL_USHORT, L"day", L"is the day of the date")
    .Category(CATEGORY)
    .FunctionHelp(L"Drop-in replacement for Excel DATE.")
);
double WINAPI xll_date(short y, unsigned short m, unsigned short d)
{
#pragma XLLEXPORT
    year_month_day ymd(year{y}, month{m}, day{d});

    return 1 + duration<double,days::period>(sys_days(ymd) - sys_days(excel_epoch)).count();

}

static AddIn xai_time(
    Function(XLL_DOUBLE, L"?xll_time", L"XLL.TIME")
    .Arg(XLL_SHORT, L"hour", L"is the hour of the time")
    .Arg(XLL_SHORT, L"minute", L"is the minute of the time")
    .Arg(XLL_SHORT, L"second", L"is the second of the time")
    .Category(CATEGORY)
    .FunctionHelp(L"Drop-in replacement for Excel TIME.")
);
double WINAPI xll_time(short h, short m, short s)
{
#pragma XLLEXPORT
    return h/24. + m/(24*60.) + s/(24*60*60.);
}

#ifdef _DEBUG

double test_day_count()
{
    year_month_day t0 = 2017_y/oct/1;
    days tplus{2};
    auto t1 = increment(t0, tplus);
    auto dt = sys_days(t1) - sys_days(t0); 
    auto x = duration<double,years::period>(dt);

    return x.count();
}

xll::test test_xll_date([]() {
    year_month_day t0 = 2017_y/oct/1;
    year_month_day t1 = 2018_y/dec/4;

    ensure (dcf<_30_360>(t0, t1) == 1 + 2/30. + 3/360.);
    ensure (dcf<actual_360>(t0, t1) == 429/360.);
    ensure (dcf<actual_365>(t0, t1) == 429/365.);

    double dt;
    dt = test_day_count();
});

#endif // _DEBUG