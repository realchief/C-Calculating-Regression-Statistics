// 11. DateTimeSpan construction and conversion operator
//     Construction tests:
//     a) With default constructor (value must be 0)
//     b) From __int64 value 126227980799999
//     c) From __int64 value -126227980799999
//     d) From DateTime objects with values 00:00:00.000 1st Mar 0004 and
//        23:59:59.999 29th Feb 4004 (value must be 126227980799999)
//     e) From DateTime objects with values 23:59:59.999 29th Feb 4004 and
//        00:00:00.000 1st Mar 0004 (value must be -126227980799999)
//     f) Using copy constructor, from object constructed in d)
//        (value must be 126227980799999)

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
DateTimeSpanTest1::test()
{
    bool ret = true;
    bool ok;

    // a) With default constructor (value must be 0)
    DateTimeSpan a;
    cout << "  DateTimeSpan a: value = " << a.totalMillis();
    ok = (a.totalMillis() == 0);
    cout << " (expected 0): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // b) From __int64 value 126227980799999
    DateTimeSpan b(126227980799999);
    cout << "  DateTimeSpan b(126227980799999): value = " << b.totalMillis();
    ok = (b.totalMillis() == 126227980799999);
    cout << " (expected 126227980799999): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // c) From __int64 value -126227980799999
    DateTimeSpan c(-126227980799999);
    cout << "  DateTimeSpan c(-126227980799999): value = " << c.totalMillis();
    ok = (c.totalMillis() == -126227980799999);
    cout << " (expected -126227980799999): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // d) From DateTime objects with values 00:00:00.000 1st Mar 1400 and
    //    23:59:59.999 29th Feb 4004 (value must be 82174348799999)
    DateTimeSpan d(DateTime(14000301, 0), DateTime(40040229, 235959999));
    cout << "  DateTimeSpan d(DateTime(14000301, 0)," << endl
         << "                 DateTime(40040229, 235959999)): value = "
         << d.totalMillis();
	//boost::int64_t x1 = d.totalMillis();
    ok = (d.totalMillis() == 82174348799999);
    cout << " (expected 82174348799999): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // e) From DateTime objects with values 23:59:59.999 29th Feb 4004 and
    //    00:00:00.000 1st Mar 1400 (value must be -82174348799999)
    DateTimeSpan e(DateTime(40040229, 235959999), DateTime(14000301, 0));
    cout << "  DateTimeSpan e(DateTime(40040229, 235959999)," << endl
         << "                 DateTime(14000301, 0)): value = " << e.totalMillis();
	//boost::int64_t x2 = e.totalMillis();
    ok = (e.totalMillis() == -82174348799999);
    cout << " (expected -82174348799999): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // f) Using copy constructor, from object constructed in d)
    //    (value must be 82174348799999)
    DateTimeSpan f(d);
    cout << "  DateTimeSpan f(d): value = " << f.totalMillis();
	//boost::int64_t x3 = f.totalMillis();
    ok = (f.totalMillis() == 82174348799999);
    cout << " (expected 82174348799999): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
