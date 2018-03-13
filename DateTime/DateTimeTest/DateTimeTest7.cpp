// 7. DateTime conversion operators
//    a) Construct DateTime object with 00:00:00.000 1st Jan 1970
//    b) Construct DateTime object with 12:06:03.000 15th Aug 2002
//    c) Construct DateTime object with 00:00:00.000 1st Jan 100
//    d) Construct DateTime object with 23:59:59.999 31st Dec 1969
//    e) Construct DateTime object with 23:59:59.999 31st Dec 0099
//    f) Get number of seconds since 1970 for each of the above objects - must
//       succeed (display results)
//    g) Attempt to convert above objects to COleDateTime - must fail with last
//       object (display result for successful conversions)
//    h) Attempt to convert above objects to CTime - must fail with last three
//       (display result for successful conversions)

#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::setprecision;

using mr::DateTime;
using mr::DateTimeSpan;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeTest7::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTime object with 00:00:00.000 1st Jan 1970
    // b) Construct DateTime object with 12:06:03.000 15th Aug 2002
    // c) Construct DateTime object with 00:00:00.000 1st Jan 1400
    // d) Construct DateTime object with 23:59:59.999 31st Dec 1969
    // e) Construct DateTime object with 23:59:59.999 31st Dec 1400
	// f) Construct DateTime object with 00:00:00.000 1st Jan 4004
    DateTime a(19700101, 0);
    DateTime b(20020815, 120603000);
    DateTime c(14000101, 0);
    DateTime d(19691231, 235959999);
    DateTime e(14001231, 235959999);
	DateTime f(20380119, 0);

    cout << "  5 DateTime objects: a (01 Jan 1970  00:00:00.000)" << endl
         << "                      b (15 Aug 2002  12:06:03.000)" << endl
         << "                      c (01 Jan 1400  00:00:00.000)" << endl
         << "                      d (31 Dec 1969  23:59:59.999)" << endl
         << "                      e (31 Dec 1400  23:59:59.999)" << endl
		 << "                      f (19 Jan 2038  00:00:00.000)" << endl
         << endl << "Conversion to seconds since 1970:" << endl;

    // f) Attempt to convert above objects to time_t - must fail with last
    //    three (display result for successful conversions)
    try {
        cout << "    a.secsSince1970() = ";
        mr::int64 result = a.secsSince1970();
        cout << result;
        ok = (result == 0);
    } catch (std::runtime_error) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 0): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    try {
        cout << "    b.secsSince1970() = ";
        mr::int64 result = b.secsSince1970();
        cout << result;
        ok = (result == 1029413163);
    } catch (std::runtime_error) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 1029413163): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    try {
        cout << "    c.secsSince1970() = ";
        mr::int64 result = c.secsSince1970();
        cout << result;
        ok = (result == -17987443200);
    } catch (std::runtime_error) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected -17987443200): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    try {
        cout << "    d.secsSince1970() = ";
        mr::int64 result = d.secsSince1970();
        cout << result;
        ok = (result == 0);
    } catch (std::runtime_error) {
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected 0): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    try
	{
        cout << "    e.secsSince1970() = ";
        mr::int64 result = e.secsSince1970();
        cout << result;
        ok = (result == -17955907200);
    }
	catch (std::runtime_error&)
	{
        cout << "<failed>";
        ok = false;
    }

    cout << " (expected -17955907200): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

	try
	{
		cout << "    f.secsSince1970() = ";
		mr::int64 result = f.secsSince1970();
		cout << result;

		ok = (result == 2147472000);
	}
	catch (std::runtime_error&)
	{
		cout << "<failed>";
		ok = false;
	}

    cout << setprecision(6);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
