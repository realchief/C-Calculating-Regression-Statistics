// 1. DateTime construction
//   Construction tests:
//   a) 00:00:00 1st Jan 1970, using default constructor (must succeed)
//   b) 23:59:59 31st Dec 1970, using time_t/__time64_t constructor
//      (must succeed)
//   c) 23:59:59 31st Dec 1970, using copy constructor with second object
//      (must succeed)
//   d) 13:52:30 14th Aug 2002, using COleDateTime constructor (must succeed)
//   e) 13:52:31 14th Aug 2002, using CTime constructor (must succeed)
//   f) 00:00:00.000 1st Mar 0004, using yr/mth/dy/h/m/s/ms constructor
//      (must succeed)
//   g) 00:00:00.000 29th Feb 4004, using yr/mth/dy/h/m/s/ms constructor
//      (must succeed)
//   h) 23:59:59.999 29th Feb 4004, using date/time constructor (must succeed)
//   i) 23:59:59.999 29th Feb 0004, using yr/mth/dy/h/m/s/ms constructor
//      (must fail)
//   j) 00:00:00.000 1st Mar 4004, using yr/mth/dy/h/m/s/ms constructor
//      (must fail)
//   k) 23:59:59.999 29th Feb 0003, using date/time constructor (must fail)
//   l) 00:00:00.000 1st Mar 4004, using date/time constructor (must fail)
//   m) 00:00:00.000 1st Mar 4004, using COleDateTime constructor (must fail)

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
DateTimeTest1::test()
{
    bool ret = true;
    bool ok;

    // a) 00:00:00 1st Jan 1970, using default constructor (must succeed)
    try {
        cout << "  DateTime dt: ";
        DateTime dt(DateTime::DT_TM_EPOCH);
        cout << "value = " << toUInt64(dt);
        ok = (toUInt64(dt) == 19700101000000000);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 19700101000000000): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // b) 23:59:59 31st Dec 1970, using time_t/__time64_t constructor
    //    (must succeed)
    try {
        cout << "  DateTime dt(31535999): ";
		DateTime dt = DateTime::fromTime_t(31535999);
        cout << "value = " << toUInt64(dt);
        ok = (toUInt64(dt) == 19701231235959000);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 19701231235959000): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // c) 23:59:59 31st Dec 1970, using copy constructor with second object
    //    (must succeed)
    try {
        cout << "  DateTime orig(31535999); DateTime dt(orig): ";
		DateTime orig = DateTime::fromTime_t(31535999);
        DateTime dt(orig);
        cout << "value = " << toUInt64(dt);
        ok = (toUInt64(dt) == 19701231235959000);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 19701231235959000): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // f) 00:00:00.000 1st Mar 1400, using yr/mth/dy/h/m/s/ms constructor
    //    (must succeed)
    try {
        cout << "  DateTime dt(1400, 3, 1, 0, 0, 0, 0): ";
        DateTime dt(1400, 3, 1, 0, 0, 0, 0);
        cout << "value = " << toUInt64(dt);
        ok = (toUInt64(dt) == 14000301000000000);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 14000301000000000): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // g) 00:00:00.000 29th Feb 4004, using yr/mth/dy/h/m/s/ms constructor
    //    (must succeed)
    try {
        cout << "  DateTime dt(4004, 2, 29, 0, 0, 0, 0): ";
        DateTime dt(4004, 2, 29, 0, 0, 0, 0);
        cout << "value = " << toUInt64(dt);
        ok = (toUInt64(dt) == 40040229000000000);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 40040229000000000): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // h) 23:59:59.999 29th Feb 4004, using date/time constructor
    //    (must succeed)
    try {
        cout << "  DateTime dt(40040229, 235959999): ";
        DateTime dt(40040229, 235959999);
        cout << "value = " << toUInt64(dt);
        ok = (toUInt64(dt) == 40040229235959999);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 40040229235959999): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // i) 23:59:59.999 29th Feb 0003, using yr/mth/dy/h/m/s/ms constructor
    //    (must fail)
    try
	{
        cout << "  DateTime dt(3, 2, 29, 23, 59, 59, 999): ";
        DateTime dt(3, 2, 29, 23, 59, 59, 999);
        cout << "value = " << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (boost::gregorian::bad_year&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // j) 00:00:00.000 1st Mar 4004, using yr/mth/dy/h/m/s/ms constructor
    try
	{
        cout << "  DateTime dt(4004, 3, 1, 0, 0, 0, 0): ";
        DateTime dt(4004, 3, 1, 0, 0, 0, 0);
        cout << "value = " << toUInt64(dt);
        ok = true;
    }
	catch (...)
	{
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // k) 23:59:59.999 29th Feb 0003, using date/time constructor (must fail)
    try {
        cout << "  DateTime dt(30229, 235959999): ";
        DateTime dt(30229, 235959999);
        cout << "value = " << toUInt64(dt);
        cout << "<failed>";
        ok = false;
	} catch (boost::gregorian::bad_year&) {
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // l) 00:00:00.000 1st Mar 4004, using date/time constructor (must not fail)
    try {
        cout << "  DateTime dt(40040301, 0): ";
        DateTime dt(40040301, 0);
        cout << "value = " << toUInt64(dt);
		ok = (toUInt64(dt) == 40040301000000000);
    } catch (...) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;
   
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
