// 14. DateTimeSpan comparison operators
//     a) Construct DateTimeSpan object 'a' with value 12345678
//     b) Construct DateTimeSpan object 'b' with value 12345677
//     c) operator==() must return true for (a == a) and (b == b) and false for
//        for (a == b) and (b == a)
//     d) operator!=() must return true for (a != b) and (b != a) and false for
//        for (a != a) and (a != b)
//     e) operator>() must return true for (a > b) and false for all other
//        combinations
//     f) operator>=() must return true for (a >= a), (a >= b) and (b >= b) and
//        false for (b >= a)
//     g) operator<() must return true for (b < a) and false for all other
//        combinations
//     h) operator<=() must return true for (a <= a), (b <= a) and (b <= b) and
//        false for (a <= b)

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

#define MR_TEST(cond, exp)                                                  \
    ok = ((cond) == exp);                                                   \
    cout << "    " #cond " = " << ((cond) ? "true" : "false")               \
         << " (expected " #exp "): " << (ok ? "OK" : "FAIL") << endl;       \
    ret = (ret && ok)

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeSpanTest4::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTimeSpan object 'a' with value 12345678
    // b) Construct DateTimeSpan object 'b' with value 12345677
    DateTimeSpan a(12345678);
    DateTimeSpan b(12345677);

    cout << "  2 DateTimeSpan objects: a (12345678)" << endl
         << "                          b (12345677)" << endl;

    // c) operator==() must return true for (a == a) and (b == b) and false for
    //    for (a == b) and (b == a)
    cout << endl << "  operator==() tests:" << endl;
    MR_TEST((a == a), true);
    MR_TEST((a == b), false);
    MR_TEST((b == a), false);
    MR_TEST((b == b), true);

    // d) operator!=() must return true for (a != b) and (b != a) and false for
    //    for (a != a) and (a != b)
    cout << endl << "  operator!=() tests:" << endl;
    MR_TEST((a != a), false);
    MR_TEST((a != b), true);
    MR_TEST((b != a), true);
    MR_TEST((b != b), false);

    // e) operator>() must return true for (a > b) and false for all other
    //    combinations
    cout << endl << "  operator>() tests:" << endl;
    MR_TEST((a > a), false);
    MR_TEST((a > b), true);
    MR_TEST((b > a), false);
    MR_TEST((b > b), false);

    // f) operator>=() must return true for (a >= a), (a >= b) and (b >= b) and
    //    false for (b >= a)
    cout << endl << "  operator>=() tests:" << endl;
    MR_TEST((a >= a), true);
    MR_TEST((a >= b), true);
    MR_TEST((b >= a), false);
    MR_TEST((b >= b), true);

    // g) operator<() must return true for (b < a) and false for all other
    //    combinations
    cout << endl << "  operator<() tests:" << endl;
    MR_TEST((a < a), false);
    MR_TEST((a < b), false);
    MR_TEST((b < a), true);
    MR_TEST((b < b), false);

    // h) operator<=() must return true for (a <= a), (b <= a) and (b <= b) and
    //    false for (a <= b)
    cout << endl << "  operator<=() tests:" << endl;
    MR_TEST((a <= a), true);
    MR_TEST((a <= b), false);
    MR_TEST((b <= a), true);
    MR_TEST((b <= b), true);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
