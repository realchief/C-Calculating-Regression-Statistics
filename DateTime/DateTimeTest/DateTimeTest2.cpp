// 2. DateTime getter functions
//    a) Construct DateTime object with 23:30:40.789 1st Dec 2002
//    b) Test individual getters: year(), month(), day() etc.
//    c) Test parameterless getDate()/getTime()
//    d) Test parameterised getDate()/getTime()
//    e) Test getIndex() function

#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using mr::DateTime;
using mr::DateTimeSpan;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeTest2::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTime object with 23:30:40.789 1st Dec 2002
    DateTime dt(2002, 12, 1, 23, 30, 40, 789);
    cout << "DateTime object 'dt' set to 23:30:40.789, 1st Dec 2002:" << endl;

    // b) Test individual getters: year(), month(), etc.
    auto year = dt.year();
    ok = (year == 2002);
    cout << "  results of year() = " << (long) year << " (expected 2002): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto month = dt.month();
    ok = (month == 12);
    cout << "  results of month() = " << (long) month << " (expected 12): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto day = dt.day();
    ok = (day == 1);
    cout << "  results of day() = " << (long) day << " (expected 1): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto day_of_week = dt.dayOfWeek();
    ok = (day_of_week == 1);
    cout << "  results of dayOfWeek() = " << (long) day_of_week << '['
         << DateTime::DAY_ABBREV[day_of_week] << "] (expected 1 [Sunday]): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto hour = dt.hour();
    ok = (hour == 23);
    cout << "  results of hour() = " << (long) hour << " (expected 23): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto minute = dt.minute();
    ok = (minute == 30);
    cout << "  results of minute() = " << (long) minute << " (expected 30): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto sec = dt.sec();
    ok = (sec == 40);
    cout << "  results of sec() = " << (long) sec << " (expected 40): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto milli = dt.milli();
    ok = (milli == 789);
    cout << "  results of milli() = " << (long) milli << " (expected 789): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // c) Test parameterless getDate()/getTime()
    auto date = dt.getDate();
    ok = (date == 20021201);
    cout << "  results of getDate() = " << date << " (expected 20021201): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    auto time = dt.getTime();
    ok = (time == 233040789);
    cout << "  results of getTime() = " << time << " (expected 233040789): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // d) Test parameterised getDate()/getTime()
    dt.getDate(year, month, day);
    ok = (year == 2002 && month == 12 && day == 1);
    cout << "  results of getDate(year, month, day) = (" << (long) year << ", "
         << (long) month << ", " << (long) day << ") (expected (2002, 12, "
         << "1)): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    dt.getTime(hour, minute, sec, milli);
    ok = (hour == 23 && minute == 30 && sec == 40 && milli == 789);
    cout << "  results of getTime(hour, minute, sec, milli) = (" << (long) hour
         << ", " << (long) minute << ", " << (long) sec << ", " << (long) milli
         << ") (expected (23, 30, 40, 789)): " << (ok ? "OK" : "FAIL")
         << endl;
    ret = ret && ok;

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
