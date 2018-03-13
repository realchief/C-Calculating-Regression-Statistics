// 5. Leap Years
//     a) Construct DateTime objects for 1600, 1800, 1970, 1992, 1993, 2000,
//        2002 and 2040
//     b) Return whether the above years are leap years using isLeapYear()
//        function

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
DateTimeTest5::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTime objects for 1600, 1800, 1970, 1992, 1993, 2000,
    //    2002 and 2040
    DateTime a(16000101, 0);
    DateTime b(18000101, 0);
    DateTime c(19700101, 0);
    DateTime d(19920101, 0);
    DateTime e(19930101, 0);
    DateTime f(20000101, 0);
    DateTime g(20020101, 0);
    DateTime h(20400101, 0);

    // b) Return whether the above years are leap years using isLeapYear()
    //    function
    ok = a.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 1600 = "
         << (a.isLeapYear() ? "true" : "false") << " (expected true): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = !b.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 1800 = "
         << (b.isLeapYear() ? "true" : "false") << " (expected false): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = !c.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 1970 = "
         << (c.isLeapYear() ? "true" : "false") << " (expected false): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = d.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 1992 = "
         << (d.isLeapYear() ? "true" : "false") << " (expected true): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = !e.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 1993 = "
         << (e.isLeapYear() ? "true" : "false") << " (expected false): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = f.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 2000 = "
         << (f.isLeapYear() ? "true" : "false") << " (expected true): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = !g.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 2002 = "
         << (g.isLeapYear() ? "true" : "false") << " (expected false): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ok = h.isLeapYear();
    cout << "  isLeapYear() on DateTime object set to year 2040 = "
         << (h.isLeapYear() ? "true" : "false") << " (expected true): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
