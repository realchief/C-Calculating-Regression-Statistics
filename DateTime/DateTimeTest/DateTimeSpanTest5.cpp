// 15. DateTimeSpan assignment operator
//     a) Construct DateTimeSpan object from DateTime objects with values
//        00:00:00.000 1st Mar 0004 and 23:59:59.999 29th Feb 4004
//     b) Construct DateTimeSpan object with default constructor
//     c) Assign first DateTimeSpan object to second DateTimeSpan object
//        (second object must have same value as first)

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
DateTimeSpanTest5::test()
{
    bool ret = true;

    // a) Construct DateTimeSpan object from DateTime objects with values
    //    00:00:00.000 1st Mar 1400 and 23:59:59.999 29th Feb 4004
    // b) Construct DateTimeSpan object with default constructor
    DateTimeSpan a(DateTime(14000301, 0), DateTime(40040229, 235959999));
    DateTimeSpan b;

    cout << "  2 DateTimeSpan objects: DateTimeSpan a(DateTime(14000301, 0), "
         << endl << "                                         "
         "DateTime(40040229, 235959999));" << endl
         << "                          DateTimeSpan b(0);" << endl << endl;

    // c) Assign first DateTimeSpan object to second DateTimeSpan object
    //    (second object must have same value as first)
    b = a;
    ret = (b == a);
    cout << "  After assignment (b = a), result of (b == a) = "
         << (ret ? "true" : "false") << " (expected true): "
         << (ret ? "OK" : "FAIL") << endl;

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
