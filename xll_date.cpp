// xll_date.cpp - date routines
#include "fms_date.h"
#include "G5260.h"

using namespace std::chrono;
using namespace date;
using namespace fms::date;
using namespace xll;

#define _T(x) L##x

// Day counts.
//XLL_ENUM_DOC(DCB_ACTUAL_YEARS, DCB_ACTUAL_YEARS, CATEGORY, _T("Time in years between dates."), _T(""))
//XLL_ENUM_DOC(DCB_30U_360, DCB_30U_360, CATEGORY, _T("Day count with 30 days per month and 360 days per year."), _T(""))
//XLL_ENU_DOC(DCB_30E_360, DCB_30E_360, CATEGORY, _T("European day count with 30 days per month and 360 days per year."), _T(""))
XLL_ENUM_DOC(DCB_30_360, DCB_30U_360, CATEGORY, _T("Day count with 30 days per month and 360 days per year."), _T(""))
XLL_ENUM_DOC(DCB_ACTUAL_360, DCB_ACTUAL_360, CATEGORY, _T("Day count is number of days divided by 360."), _T(""))
XLL_ENUM_DOC(DCB_ACTUAL_365, DCB_ACTUAL_365, CATEGORY, _T("Day count is number of days divided by 365."), _T(""))
//XLL_ENUM_DOC(DCB_ACTUAL_ACTUAL_ISDA, DCB_ACTUAL_ACTUAL_ISDA, CATEGORY, _T("Day count is number of days divided by 365, or 366 for leap years")

// Rolling conventions.
//XLL_ENUM_DOC(ROLL_NONE, ROLL_NONE, CATEGORY, _T("No roll convention."), _T(""))
XLL_ENUM_DOC(ROLL_FOLLOWING_BUSINESS, ROLL_FOLLOWING_BUSINESS, CATEGORY, _T("The following business day."), _T(""))
//XLL_ENUM_DOC(ROLL_PREVIOUS_BUSINESS, ROLL_PREVIOUS_BUSINESS, CATEGORY, _T("The previous business day."), _T(""))
XLL_ENUM_DOC(ROLL_MODIFIED_FOLLOWING, ROLL_MODIFIED_FOLLOWING, CATEGORY, _T("The earlier of following and last business day of month."), _T(""))
//XLL_ENUM_DOC(ROLL_MODIFIED_PREVIOUS, ROLL_MODIFIED_PREVIOUS, CATEGORY, _T("The later of the previous and first business day of month."), _T(""))

// Frequency
XLL_ENUM_DOC(FREQ_NO_FREQUENCY,FREQ_NO_FREQUENCY,CATEGORY,L"",L"")
XLL_ENUM_DOC(FREQ_ANNUALLY,FREQ_ANNUALLY,CATEGORY,L"Once per year.",L"")
XLL_ENUM_DOC(FREQ_SEMIANNUALLY,FREQ_SEMIANNUALLY,CATEGORY,L"Twice per year.",L"")
XLL_ENUM_DOC(FREQ_QUARTERLY,FREQ_QUARTERLY,CATEGORY,L"Four times a year.",L"")
XLL_ENUM_DOC(FREQ_MONTHLY,FREQ_MONTHLY,CATEGORY,L"Twelve times a year.",L"")
XLL_ENUM_DOC(FREQ_BIWEEKLY,FREQ_BIWEEKLY,CATEGORY,L"Twenty six times a year.",L"")
XLL_ENUM_DOC(FREQ_WEEKLY,FREQ_WEEKLY,CATEGORY,L"Fifty two times per year.",L"")

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

    return excel_date(ymd);
}

static AddIn xai_yyyymmdd(
    Function(XLL_DOUBLE, L"?xll_yyyymmdd", L"XLL.YYYYMMDD")
    .Arg(XLL_DOUBLE, L"date", L"is an Excel date")
    .Category(CATEGORY)
    .FunctionHelp(L"Convert date to yyyymmdd integer.")
);
double WINAPI xll_yyyymmdd(double date)
{
#pragma XLLEXPORT
    year_month_day ymd = excel_date(date);
    int y = static_cast<int>(ymd.year());
    unsigned m = static_cast<unsigned>(ymd.month());
    unsigned d = static_cast<unsigned>(ymd.day());

    return 10000*y + 100*m + d;
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

xll::test test_xll_date([]() {
    year_month_day t0 = 2017_y/oct/1;
    year_month_day t1 = 2018_y/dec/4;

    ensure (dcf<_30_360>(t0, t1) == 1 + 2/30. + 3/360.);
    ensure (dcf<actual_360>(t0, t1) == 429/360.);
    ensure (dcf<actual_365>(t0, t1) == 429/365.);
});

#endif // _DEBUG