// 12. DateTimeSpan getter functions
//     a) Construct DateTimeSpan object from DateTime objects with values
//        00:00:00.000 1st Mar 0004 and 23:59:59.999 29th Feb 4004
//     b) Test days(), hours(), totalHours() etc.

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

/*****  Macros and Constants  *****/

#define MR_TEST(func, exp)                                                  \
    val = (func);                                                           \
    ok = (val == exp);                                                      \
    cout << "    " #func " = " << val << " (expected " << exp << "): "      \
         << (ok ? "OK" : "FAIL") << endl;                                   \
    ret = (ret && ok)

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeSpanTest2::test()
{
    bool ret = true;
    bool ok;
    __int64 val;

    // a) Construct DateTimeSpan object from DateTime objects with values
    //    00:00:00.000 1st Mar 1400 and 23:59:59.999 29th Feb 4004
    DateTimeSpan a(DateTime(14000301, 0), DateTime(40040229, 235959999));

    cout << "  With DateTimeSpan a(DateTime(14000301, 0), "
            "DateTime(40040229, 235959999)):" << endl;

    // b) Test days(), hours(), totalHours() etc.
    MR_TEST(a.days(), 951091);
    MR_TEST(a.hours(), 23);
    MR_TEST(a.totalHours(), 22826207);
    MR_TEST(a.mins(), 59);
    MR_TEST(a.totalMins(), 1369572479);
    MR_TEST(a.secs(), 59);
    MR_TEST(a.totalSecs(), 82174348799);
    MR_TEST(a.millis(), 999);
    MR_TEST(a.totalMillis(), 82174348799999);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
