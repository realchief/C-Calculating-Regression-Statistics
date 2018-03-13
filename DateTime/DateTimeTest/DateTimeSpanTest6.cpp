// 16. DateTimeSpan arithmetic operators
//     a) Construct DateTimeSpan object 'a' with value 1000
//     b) Construct DateTimeSpan object 'b' with value -3000
//     c) Construct DateTimeSpan object 'c' with value 126227980799999
//     d) Construct DateTimeSpan object 'd' with value -126227980799999
//     e) Construct DateTimeSpan object 'e' with default constructor
//     f) (e = a + b) must yield -2000 for e; a and b must be unchanged
//     g) (e = a - b) must yield 4000 for e; a and b must be unchanged
//     h) (e = c + b) must yield 126227980796999 for e; c and b must be
//        unchanged
//     i) (e = d - b) must yield -126227980796999 for e; d and b must be
//        unchanged
//     j) (a += b) must yield -2000 for a; b must be unchanged
//     k) (b -= a) must yield -1000 for b; a must be unchanged
//     l) (c += b) must yield 126227980798999 for c; b must be unchanged
//     m) (a -= d) must yield 126227980797999 for a; d must be unchanged

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

#define MR_TEST1(lhs, p1, op, p2, exp)                                      \
    try {                                                                   \
        DateTimeSpan orig1(p1);                                             \
        DateTimeSpan orig2(p2);                                             \
                                                                            \
        cout << "  Result of (" #lhs " = " #p1 " " #op " " #p2 ") is ";     \
                                                                            \
        lhs = p1 op p2;                                                     \
        ok = (lhs.totalMillis() == exp) && (p1 == orig1) && (p2 == orig2);  \
        cout << lhs.totalMillis();                                          \
    } catch (std::runtime_error) {                                          \
        cout << "<failed>";                                                 \
        ok = false;                                                         \
    }                                                                       \
                                                                            \
    cout << " (expected " << exp << "; " #p1 " & " #p2 " unchanged): "      \
         << (ok ? "OK" : "FAIL") << endl;                                   \
    ret = (ret && ok);


#define MR_TEST2(lhs, op, rhs, exp)                                         \
    try {                                                                   \
        DateTimeSpan orig(rhs);                                             \
                                                                            \
        cout << "  Result of (" #lhs " " #op " " #rhs ") is ";              \
                                                                            \
        lhs op rhs;                                                         \
        ok = (lhs.totalMillis() == exp) && (rhs == orig);                   \
        cout << lhs.totalMillis();                                          \
    } catch (std::runtime_error) {                                          \
        cout << "<failed>";                                                 \
        ok = false;                                                         \
    }                                                                       \
                                                                            \
    cout << " (expected " << exp << "; " #rhs " unchanged): "               \
         << (ok ? "OK" : "FAIL") << endl;                                   \
    ret = (ret && ok);

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeSpanTest6::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTimeSpan object 'a' with value 1000
    // b) Construct DateTimeSpan object 'b' with value -3000
    // c) Construct DateTimeSpan object 'c' with value 126227980799999
    // d) Construct DateTimeSpan object 'd' with value -126227980799999
    // e) Construct DateTimeSpan object 'e' with default constructor
    DateTimeSpan a(1000);
    DateTimeSpan b(-3000);
    DateTimeSpan c(126227980799999);
    DateTimeSpan d(-126227980799999);
    DateTimeSpan e(1);
    DateTimeSpan f;

    cout << "  5 DateTimeSpan objects: a (1000)" << endl
         << "                          b (-3000)" << endl
         << "                          c (126227980799999)" << endl
         << "                          d (-126227980799999)" << endl
         << "                          e (0)" << endl << endl;

    // f) (e = a + b) must yield -2000 for e; a and b must be unchanged
    MR_TEST1(e, a, +, b, -2000);

    // g) (e = a - b) must yield 4000 for e; a and b must be unchanged
    MR_TEST1(e, a, -, b, 4000);

    // h) (e = c + b) must yield 126227980796999 for e; c and b must be
    //    unchanged
    MR_TEST1(e, c, +, b, 126227980796999);

    // i) (e = d - b) must yield -126227980796999 for e; d and b must be
    //    unchanged
    MR_TEST1(e, d, -, b, -126227980796999);

    // j) (a += b) must yield -2000 for a; b must be unchanged
    MR_TEST2(a, +=, b, -2000);

    // k) (b -= a) must yield -1000 for b; a must be unchanged
    MR_TEST2(b, -=, a, -1000);

    // l) (c += b) must yield 126227980798999 for c; b must be unchanged
    MR_TEST2(c, +=, b, 126227980798999);

    // m) (a -= d) must yield 126227980797999 for a; d must be unchanged
    MR_TEST2(a, -=, d, 126227980797999);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
