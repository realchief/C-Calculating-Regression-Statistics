// 8. DateTime comparison operators
//    a) Construct DateTime object 'a' with 00:00:00.000 1st Jan 1970
//    b) Construct DateTime object 'b' with 23:59:59.999 31st Dec 1969
//    c) Construct DateTime object 'c' with 00:00:00.001 1st Jan 1970
//    d) Construct DateTime object 'd' with 00:00:00.000 2nd Jan 1970
//    e) operator==() must return true for (a == a), (b == b), (c == c) and
//       (d == d) but false for all other combinations
//    f) operator!=() must return false for (a != a), (b != b), (c != c) and
//       (d != d) but true for all other combinations
//    g) operator<() must return true for (a < c), (a < d), (b < a), (b < c),
//       (b < d) and (c < d) but false for all other combinations
//    h) operator<=() must return true for (a <= a), (a <= c), (a <= d),
//       (b <= a), (b <= b), (b <= c), (b <= d), (c <= c), (c <= d) and
//       (d <= d) but false for all other combinations
//    i) operator>() must return true for (a > b), (c > a), (c > b), (d > a),
//       (d > b) and (d > c) but false for all other combinations
//    j) operator>=() must return true for (a >= a), (a >= b), (b >= b),
//       (c >= a), (c >= b), (c >= c), (d >= a), (d >= b), (d >= c) and
//       (d >= d) but false for all other combinations

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
DateTimeTest8::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTime object 'a' with 00:00:00.000 1st Jan 1970
    // b) Construct DateTime object 'b' with 23:59:59.999 31st Dec 1969
    // c) Construct DateTime object 'c' with 00:00:00.001 1st Jan 1970
    // d) Construct DateTime object 'd' with 00:00:00.000 2nd Jan 1970
    DateTime a(19700101, 0);
    DateTime b(19691231, 235959999);
    DateTime c(19700101, 1);
    DateTime d(19700102, 0);

    cout << "  4 DateTime objects: a (01 Jan 1970  00:00:00.000)" << endl
         << "                      b (31 Dec 1969  23:59:59.999)" << endl
         << "                      c (01 Jan 1970  00:00:00.001)" << endl
         << "                      d (02 Jan 1970  00:00:00.000)" << endl;

    // e) operator==() must return true for (a == a), (b == b), (c == c) and
    //    (d == d) but false for all other combinations
    cout << endl << "  operator==() tests:" << endl;
    MR_TEST((a == a), true);
    MR_TEST((a == b), false);
    MR_TEST((a == c), false);
    MR_TEST((a == d), false);
    MR_TEST((b == a), false);
    MR_TEST((b == b), true);
    MR_TEST((b == c), false);
    MR_TEST((b == d), false);
    MR_TEST((c == a), false);
    MR_TEST((c == b), false);
    MR_TEST((c == c), true);
    MR_TEST((c == d), false);
    MR_TEST((d == a), false);
    MR_TEST((d == b), false);
    MR_TEST((d == c), false);
    MR_TEST((d == d), true);

    // f) operator!=() must return false for (a != a), (b != b), (c != c) and
    //    (d != d) but true for all other combinations
    cout << endl << "  operator!=() tests:" << endl;
    MR_TEST((a != a), false);
    MR_TEST((a != b), true);
    MR_TEST((a != c), true);
    MR_TEST((a != d), true);
    MR_TEST((b != a), true);
    MR_TEST((b != b), false);
    MR_TEST((b != c), true);
    MR_TEST((b != d), true);
    MR_TEST((c != a), true);
    MR_TEST((c != b), true);
    MR_TEST((c != c), false);
    MR_TEST((c != d), true);
    MR_TEST((d != a), true);
    MR_TEST((d != b), true);
    MR_TEST((d != c), true);
    MR_TEST((d != d), false);

    // g) operator<() must return true for (a < c), (a < d), (b < a), (b < c),
    //    (b < d) and (c < d) but false for all other combinations
    cout << endl << "  operator<() tests:" << endl;
    MR_TEST((a < a), false);
    MR_TEST((a < b), false);
    MR_TEST((a < c), true);
    MR_TEST((a < d), true);
    MR_TEST((b < a), true);
    MR_TEST((b < b), false);
    MR_TEST((b < c), true);
    MR_TEST((b < d), true);
    MR_TEST((c < a), false);
    MR_TEST((c < b), false);
    MR_TEST((c < c), false);
    MR_TEST((c < d), true);
    MR_TEST((d < a), false);
    MR_TEST((d < b), false);
    MR_TEST((d < c), false);
    MR_TEST((d < d), false);

    // h) operator<=() must return true for (a <= a), (a <= c), (a <= d),
    //    (b <= a), (b <= b), (b <= c), (b <= d), (c <= c), (c <= d) and
    //    (d <= d) but false for all other combinations
    cout << endl << "  operator<=() tests:" << endl;
    MR_TEST((a <= a), true);
    MR_TEST((a <= b), false);
    MR_TEST((a <= c), true);
    MR_TEST((a <= d), true);
    MR_TEST((b <= a), true);
    MR_TEST((b <= b), true);
    MR_TEST((b <= c), true);
    MR_TEST((b <= d), true);
    MR_TEST((c <= a), false);
    MR_TEST((c <= b), false);
    MR_TEST((c <= c), true);
    MR_TEST((c <= d), true);
    MR_TEST((d <= a), false);
    MR_TEST((d <= b), false);
    MR_TEST((d <= c), false);
    MR_TEST((d <= d), true);

    // i) operator>() must return true for (a > b), (c > a), (c > b), (d > a),
    //    (d > b) and (d > c) but false for all other combinations
    cout << endl << "  operator>() tests:" << endl;
    MR_TEST((a > a), false);
    MR_TEST((a > b), true);
    MR_TEST((a > c), false);
    MR_TEST((a > d), false);
    MR_TEST((b > a), false);
    MR_TEST((b > b), false);
    MR_TEST((b > c), false);
    MR_TEST((b > d), false);
    MR_TEST((c > a), true);
    MR_TEST((c > b), true);
    MR_TEST((c > c), false);
    MR_TEST((c > d), false);
    MR_TEST((d > a), true);
    MR_TEST((d > b), true);
    MR_TEST((d > c), true);
    MR_TEST((d > d), false);

    // j) operator>=() must return true for (a >= a), (a >= b), (b >= b),
    //    (c >= a), (c >= b), (c >= c), (d >= a), (d >= b), (d >= c) and
    //    (d >= d) but false for all other combinations
    cout << endl << "  operator>=() tests:" << endl;
    MR_TEST((a >= a), true);
    MR_TEST((a >= b), true);
    MR_TEST((a >= c), false);
    MR_TEST((a >= d), false);
    MR_TEST((b >= a), false);
    MR_TEST((b >= b), true);
    MR_TEST((b >= c), false);
    MR_TEST((b >= d), false);
    MR_TEST((c >= a), true);
    MR_TEST((c >= b), true);
    MR_TEST((c >= c), true);
    MR_TEST((c >= d), false);
    MR_TEST((d >= a), true);
    MR_TEST((d >= b), true);
    MR_TEST((d >= c), true);
    MR_TEST((d >= d), true);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
