// 17. DateTime arithmetic operators
//     a) Construct DateTime object 'a' with 00:00:00.000 1st Mar 0004
//     b) Construct DateTime object 'b' with 23:59:59.999 29th Feb 4004
//     c) Construct DateTime object 'c' with default constructor
//     d) Construct DateTime object 'd' with default constructor
//     e) Construct DateTime object 'e' with 00:00:00.000 1st Mar 0004
//     f) Construct DateTime object 'f' with 23:59:59.999 29th Feb 4004
//     g) Construct DateTimeSpan object 'g' with value 126227980799999
//     h) Construct DateTimeSpan object 'h' with value -126227980799999
//     i) Construct DateTimeSpan object 'i' with value 62035977600000
//     j) Construct DateTimeSpan object 'j' with value 1
//     k) Construct DateTimeSpan object 'k' with value -1
//     l) After (d = a + i), d must equal c; a and i must be unchanged
//     m) After (d = a + g), d must equal b; a and g must be unchanged
//     n) (d = a + k) must fail; all values must be unchanged
//     o) (d = b + j) must fail; all values must be unchanged
//     p) After (d = b - h), d must equal a; b and h must be unchanged
//     q) After (d = c - i), d must equal a; c and i must be unchanged
//     r) (d = a - j) must fail; all values must be unchanged
//     s) (d = b + j) must fail; all values must be unchanged
//     t) (a += k) must fail; all values must be unchanged
//     u) After (a += g), a must equal f; g must be unchanged
//     v) After (a += h), a must equal e; h must be unchanged
//     w) (b -= k) must fail; all values must be unchanged
//     x) After (b -= g), b must equal e; g must be unchanged
//     y) After (b -= h), b must equal f; h must be unchanged

#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using namespace boost;
using namespace boost::posix_time;
using namespace std;

using mr::DateTime;
using mr::DateTimeSpan;

///////////////////////////////////////////////////////////////////////////////

/*****  Macros and Constants  *****/

inline bool testPlus(const DateTime& lhs, const DateTimeSpan& rhs)
{
	return (lhs + rhs).asPtime() == lhs.asPtime() + rhs.toPosixDuration();
}

inline bool testMinus(const DateTime& lhs, const DateTimeSpan& rhs)
{
	return (lhs - rhs).asPtime() == lhs.asPtime() - rhs.toPosixDuration();
}

inline bool testPlusEquals(const DateTime& lhs, const DateTimeSpan& rhs)
{
	DateTime result(lhs);
	result += rhs;

	ptime reference_result(lhs.asPtime());
	reference_result += rhs.toPosixDuration();

	return (result == (lhs + rhs))
		&& (result.asPtime() == reference_result);
}

inline bool testMinusEquals(const DateTime& lhs, const DateTimeSpan& rhs)
{
	DateTime result(lhs);
	result -= rhs;

	ptime reference_result(lhs.asPtime());
	reference_result -= rhs.toPosixDuration();

	return (result == (lhs - rhs))
		&& (result.asPtime() == reference_result);
}

#define MR_TEST_SUCCESS_1(lhs, p1, op, p2, exp)                             \
    try {                                                                   \
        DateTime orig1(p1);                                                 \
        DateTimeSpan orig2(p2);                                             \
                                                                            \
        cout << "  Result of (" #lhs " = " #p1 " " #op " " #p2 ") is ";     \
        lhs = p1 op p2;                                                     \
        ok = (lhs == exp) && (p1 == orig1) && (p2 == orig2);                \
                                                                            \
        if (lhs == exp) {                                                   \
            cout << #exp;                                                   \
        } else {                                                            \
            cout << toUInt64(lhs);                                          \
        }                                                                   \
    } catch (std::runtime_error) {                                          \
        cout << "<failed>";                                                 \
        ok = false;                                                         \
    }                                                                       \
                                                                            \
    cout << " (expected " #exp "; " #p1 " & " #p2 " unchanged): "           \
         << (ok ? "OK" : "FAIL") << endl;                                   \
    ret = (ret && ok);


#define MR_TEST_SUCCESS_2(lhs, op, rhs, exp)                                \
    try {                                                                   \
        DateTimeSpan orig(rhs);                                             \
                                                                            \
        cout << "  Result of (" #lhs " " #op " " #rhs ") is ";              \
        lhs op rhs;                                                         \
        ok = (lhs == exp) && (rhs == orig);                                 \
                                                                            \
        if (lhs == exp) {                                                   \
            cout << #exp;                                                   \
        } else {                                                            \
            cout << toUInt64(lhs);                                          \
        }                                                                   \
    } catch (std::runtime_error) {                                          \
        cout << "<failed>";                                                 \
        ok = false;                                                         \
    }                                                                       \
                                                                            \
    cout << " (expected " #exp "; " #rhs " unchanged): "                    \
         << (ok ? "OK" : "FAIL") << endl;                                   \
    ret = (ret && ok);


#define MR_TEST_FAILURE_1(lhs, p1, op, p2)                                  \
    {                                                                       \
        DateTime orig1(lhs);                                                \
        DateTime orig2(p1);                                                 \
        DateTimeSpan orig3(p2);                                             \
                                                                            \
        try {                                                               \
            cout << "  Result of (" #lhs " = " #p1 " " #op " " #p2 ") is "; \
            lhs = p1 op p2;                                                 \
            ok = false;                                                     \
            cout << toUInt64(lhs);                                          \
        } catch (std::runtime_error) {                                      \
            cout << "<failed>";                                             \
            ok = (lhs == orig1) && (p1 == orig2) && (p2 == orig3);          \
        }                                                                   \
                                                                            \
        cout << " (expected failure; all values unchanged): "               \
             << (ok ? "OK" : "FAIL") << endl;                               \
        ret = (ret && ok);                                                  \
    }


#define MR_TEST_FAILURE_2(lhs, op, rhs)                                     \
    {                                                                       \
        DateTime orig1(lhs);                                                \
        DateTimeSpan orig2(rhs);                                            \
                                                                            \
        try {                                                               \
            cout << "  Result of (" #lhs " " #op " " #rhs ") is ";          \
            lhs op rhs;                                                     \
            ok = false;                                                     \
            cout << toUInt64(lhs);                                          \
        } catch (std::runtime_error) {                                      \
            cout << "<failed>";                                             \
            ok = (lhs == orig1) && (rhs == orig2);                          \
        }                                                                   \
                                                                            \
        cout << " (expected failure; all values unchanged): "               \
            << (ok ? "OK" : "FAIL") << endl;                                \
        ret = (ret && ok);                                                  \
    }

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeTest11::test()
{
    bool ok = true;

    // a) Construct DateTime object 'a' with 00:00:00.000 1st Mar 1400
    // b) Construct DateTime object 'b' with 23:59:59.999 29th Feb 4004
    // c) Construct DateTime object 'c' with default constructor
    // d) Construct DateTime object 'd' with default constructor
    // e) Construct DateTime object 'e' with 00:00:00.000 1st Mar 1400
    // f) Construct DateTime object 'f' with 23:59:59.999 29th Feb 4004
    // g) Construct DateTimeSpan object 'g' with value 126227980799999
    // h) Construct DateTimeSpan object 'h' with value -126227980799999
    // i) Construct DateTimeSpan object 'i' with value 62035977600000
    // j) Construct DateTimeSpan object 'j' with value 1
    // k) Construct DateTimeSpan object 'k' with value -1
    DateTime a(14000301, 0);
    DateTime b(40040229, 235959999);
    DateTime c;
    DateTime d;
    DateTime e(14000301, 0);
    DateTime f(40040229, 235959999);

    DateTimeSpan g(126227980799999);
    DateTimeSpan h(-126227980799999);
    DateTimeSpan i(62035977600000);
    DateTimeSpan j(1);
    DateTimeSpan k(-1);

    cout << "  6 DateTime objects: a (01 Mar 1400 00:00:00.000)" << endl
         << "                      b (29 Feb 4004 23:59:59.999)" << endl
         << "                      c (01 Jan 1970 00:00:00.000)" << endl
         << "                      d (01 Jan 1970 00:00:00.000)" << endl
         << "                      e (01 Mar 1400 00:00:00.000)" << endl
         << "                      f (29 Feb 4004 23:59:59.999)" << endl
         << endl;

    cout << "  5 DateTimeSpan objects: g (126227980799999)" << endl
         << "                          h (-126227980799999)" << endl
         << "                          i (62035977600000)" << endl
         << "                          j (1)" << endl
         << "                          k (-1)" << endl << endl;

    // l) a + i 
	ok &= testPlus(a, i);

    // m) a + g
	ok &= testPlus(a, g);

    // n) a + k
	ok &= testPlus(a, k);

    // o)  b + j
	ok &= testPlus(b, j);

    // p) After  b - g
	ok &= testMinus(b, g);

    // q)  c - i
	ok &= testMinus(c, i);

    // r) a - j
	ok &= testMinus(a, j);

    // s) (d = b + j) 
	ok &= testPlus(b, j);

    // t) (a += k) // must fail; all values must be unchanged
	ok &= testPlusEquals(a, k);

    // u) (a += g), a must equal f; g must be unchanged
	ok &= testPlusEquals(a, g);

    // v) After (a += h), a must equal e; h must be unchanged
	ok &= testPlusEquals(a, h);

    // w) (b -= k) must fail; all values must be unchanged
	ok &= testMinusEquals(b, k);

    // x) After (b -= g), b must equal e; g must be unchanged
	ok &= testMinusEquals(b, g);

    // y) After (b -= h), b must equal f; h must be unchanged
	ok &= testMinusEquals(b, h);

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
