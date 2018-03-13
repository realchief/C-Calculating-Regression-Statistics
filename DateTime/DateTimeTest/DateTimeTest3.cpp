// 3. DateTime setter functions
//    a) Construct DateTime object
//    b) With both variants of setDate()/setTime(), set date 31st July 2002,
//       time 16:29:44.987 and display in each case
//    c) Try to set date 29th Feb 2002 - must fail with both setDate()
//       functions
//    d) Try to set date 29th Feb 0003 - must fail with both setDate()
//       functions
//    e) Try to set date 1st Mar 0004 - must succeed with both setDate()
//       functions
//    f) Try to set date 29th Feb 14000 - must succeed with both setDate()
//       functions
//    g) Try to set date 1st Mar 14001 - must fail with both setDate() functions
//    h) Try to set time 24:00:00.000 - must fail with both setTime() functions
//    i) Try to set time 00:60:00.000 - must fail with both setTime() functions
//    j) Try to set time 00:00:60.000 - must fail with both setTime() functions

#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using mr::DateTime;
using mr::DateTimeSpan;

using namespace std;
using namespace boost::gregorian;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeTest3::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTime object
    DateTime dt;

    // b) With both variants of setDate()/setTime(), set date 31st July 2002,
    //    time 16:29:44.987 and display in each case
    dt.setDate(20020731);
    dt.setTime(162944987);
    ok = (toUInt64(dt) == 20020731162944987);
    cout << "  Results of setDate(20023107) and setTime(162944987) = "
         << toUInt64(dt) << " (expected 20020731162944987): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    dt.setDate(19700101);   // Reset object contents
    dt.setTime(0);

    dt.setDate(2002, 7, 31);
    dt.setTime(16, 29, 44, 987);
    ok = (toUInt64(dt) == 20020731162944987);
    cout << "  Results of setDate(2002, 7, 31) and setTime(16, 29, 44, 987) = "
         << toUInt64(dt) << " (expected 20020731162944987): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // c) Try to set date 29th Feb 2002 - must fail with both setDate() funcs
    try
	{
        cout << "  Results of setDate(20020229) = ";
        dt.setDate(20020229);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
	}
	catch (bad_day_of_month&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setDate(2002, 2, 29) = ";
        dt.setDate(2002, 2, 29);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
	}
	catch (bad_day_of_month&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // d) Try to set date 29th Feb 0003 - must fail with both setDate() funcs
    try
	{
        cout << "  Results of setDate(30229) = ";
        dt.setDate(30229);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
	}
	catch (bad_year&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setDate(3, 2, 29) = ";
        dt.setDate(3, 2, 29);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
	}
	catch (bad_year&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // e) Try to set date 1st Mar 1400 - must succeed with both setDate() funcs
    try
	{
        cout << "  Results of setDate(14000301) = ";
        dt.setDate(14000301);
        cout << toUInt64(dt);
        ok = (toUInt64(dt) == 14000301162944987);
    }
	catch (std::exception&)
	{
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 14000301162944987): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setDate(1400, 3, 1) = ";
        dt.setDate(1400, 3, 1);
        cout << toUInt64(dt);
        ok = (toUInt64(dt) == 14000301162944987);
    }
	catch (std::exception&)
	{
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 14000301162944987): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // f) Try to set date 29th Feb 10000 - must succeed with both setDate()
    //    functions
    try
	{
        cout << "  Results of setDate(100000229) = ";
        dt.setDate(100000229);
        cout << toUInt64(dt);
        ok = (toUInt64(dt) == 100000229162944987);
    }
	catch (std::exception&)
	{
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setDate(10000, 2, 29) = ";
        dt.setDate(10000, 2, 29);
        cout << toUInt64(dt);
        ok = (toUInt64(dt) == 100000229162944987);
    }
	catch (std::exception&)
	{
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // g) Try to set date 1st Mar 14001 - must fail with both setDate() funcs
    try
	{
        cout << "  Results of setDate(140010301) = ";
        dt.setDate(140010301);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (bad_year&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setDate(14001, 3, 1) = ";
        dt.setDate(14001, 3, 1);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (bad_year&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret &= ok;

    // h) Try to set time 24:00:00.000 - must fail with both setTime() funcs
    try
	{
        cout << "  Results of setTime(240000000) = ";
        dt.setTime(240000000);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (exception&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret &= ok;

    try
	{
        cout << "  Results of setTime(24, 0, 0, 0) = ";
        dt.setTime(24, 0, 0, 0);
        cout << toUInt64(dt);
        cout << "<failed>";
        ok = false;
    }
	catch (exception&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // i) Try to set time 00:60:00.000 - must fail with both setTime() funcs
    try
	{
        cout << "  Results of setTime(6000000) = ";
        dt.setTime(6000000);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (exception&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setTime(0, 60, 0, 0) = ";
        dt.setTime(0, 60, 0, 0);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (std::exception&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    // j) Try to set time 00:00:60.000 - must fail with both setTime() funcs
    try
	{
        cout << "  Results of setTime(60000) = ";
        dt.setTime(60000);
        cout << toUInt64(dt);
        ok = false;
        cout << "<failed>";
    }
	catch (std::exception&)
	{
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    try
	{
        cout << "  Results of setTime(0, 0, 60, 0) = ";
        dt.setTime(0, 0, 60, 0);
        cout << toUInt64(dt);
        ok = false;
    }
	catch (std::exception&)
	{
        cout << "<failed>";
        ok = true;
    }

    cout << " (expected failure): " << (ok ? "OK" : "FAIL") << endl;
    ret = ret && ok;

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
