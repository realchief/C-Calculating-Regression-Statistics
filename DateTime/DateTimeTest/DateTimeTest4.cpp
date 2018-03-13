// 4. DateTime's now()/Local time/GMT functions
//     a) Construct DateTime object with current date/time using now() function
//     b) Convert to local time and display
//     c) Convert back to GMT and display

#include "stdafx.h"
#include <cmath>
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using namespace boost;
using namespace std;

using mr::DateTime;
using mr::DateTimeSpan;

// Stop braindead warning on Visual C++ compiler
#if _MSC_VER
#pragma warning(disable: 4267)
#endif /* _MSC_VER */

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeTest4::test()
{
    bool ret = true;
    bool ok;
    time_t t = time(&t);
    struct tm local;
    struct tm gmt;
    struct tm *tmptr;

	// BUG FIX:
	// Modified the test harness to make the two 'now' type calls together
	// to reduce the risk of the second changing between the instructions.
	// In addition I have put a tolerance of 4 seconds in.

    // Get current local time and GMT time for later comparison
    tmptr = gmtime(&t);
    // a) Construct DateTime object with current date/time using now() function
    DateTime dt(DateTime::now());

    memcpy(&gmt, tmptr, sizeof(struct tm));
    gmt.tm_year += 1900;
    gmt.tm_mon++;

    tmptr = localtime(&t);
    memcpy(&local, tmptr, sizeof(struct tm));
    local.tm_year += 1900;
    local.tm_mon++;

    // b) Convert to local time and display
    dt.toLocalTime();

//  Cannot use DBC yet because no handler has been set up for it!
    //DBC_ALWAYS_CHECK(local.tm_year == dt.year());
	//DBC_ALWAYS_CHECK(local.tm_mon == dt.month());
	//DBC_ALWAYS_CHECK(local.tm_mday == dt.day());
	//DBC_ALWAYS_CHECK(local.tm_hour == dt.hour());
	//DBC_ALWAYS_CHECK(local.tm_min == dt.minute());
	//DBC_ALWAYS_CHECK(std::abs(numeric_cast<long>(local.tm_sec) - numeric_cast<long>(dt.sec())) <= 4);

    ok = (local.tm_year == dt.year() && local.tm_mon == dt.month() &&
          local.tm_mday == dt.day() && local.tm_hour == dt.hour() &&
          local.tm_min == dt.minute()
		  && (std::abs(numeric_cast<long>(local.tm_sec) - numeric_cast<long>(dt.sec())) < 4));

    cout << "  Current local date & time according to 'tm' :" << endl
		<< "    " << ' ' << local.tm_mday << ' '
		<< DateTime::MONTH[local.tm_mon] << ' ' << setfill('0') << setw(4)
		<< local.tm_year << ' ' << setw(2) << local.tm_hour << ':' << setw(2)
		<< local.tm_min << ':' << setw(2) << local.tm_sec << setfill(' ') << ' '
         << (ok ? "(OK)" : "(FAIL)") << endl;


    cout << "  Current local date & time according to DateTime :" << endl
         << "    " << DateTime::DAY[dt.dayOfWeek()] << ' ' << dt.day() << ' '
         << DateTime::MONTH[dt.month()] << ' ' << setfill('0') << setw(4)
         << dt.year() << ' ' << setw(2) << dt.hour() << ':' << setw(2)
         << dt.minute() << ':' << setw(2) << dt.sec() << setfill(' ') << ' '
         << (ok ? "(OK)" : "(FAIL)") << endl;

    ret = (ret && ok);

    // c) Convert back to GMT and display
    dt.toGMT();

//  Cannot use DBC yet because no handler has been set up for it!
	//DBC_ALWAYS_CHECK(gmt.tm_year == dt.year());
	//DBC_ALWAYS_CHECK(gmt.tm_mon == dt.month());
	//DBC_ALWAYS_CHECK(gmt.tm_mday == dt.day());
	//DBC_ALWAYS_CHECK(gmt.tm_hour == dt.hour());
	//DBC_ALWAYS_CHECK(gmt.tm_min == dt.minute());
	//DBC_ALWAYS_CHECK(std::abs(numeric_cast<long>(gmt.tm_sec) - numeric_cast<long>(dt.sec())) < 4);

    ok = (gmt.tm_year == dt.year() && gmt.tm_mon == dt.month() &&
          gmt.tm_mday == dt.day() && gmt.tm_hour == dt.hour() &&
          (std::abs(numeric_cast<long>(gmt.tm_sec) - numeric_cast<long>(dt.sec())) < 4));

    cout << "  Current GMT according to DateTime:" << endl << "    "
         << DateTime::DAY[dt.dayOfWeek()] << ' ' << dt.day() << ' '
         << DateTime::MONTH[dt.month()] << ' ' << setfill('0') << setw(4)
         << dt.year() << ' ' << setw(2) << dt.hour() << ':' << setw(2)
         << dt.minute() << ':' << setw(2) << dt.sec() << setfill(' ') << ' '
         << (ok ? "(OK)" : "(FAIL)") << endl;

	cout << "  Current GMT according to 'tm':" << endl << "    "
		<< ' ' << gmt.tm_mday << ' '
		<< DateTime::MONTH[gmt.tm_mon] << ' ' << setfill('0') << setw(4)
		<< gmt.tm_year << ' ' << setw(2) << gmt.tm_hour << ':' << setw(2)
		<< gmt.tm_min << ':' << setw(2) << gmt.tm_sec << setfill(' ') << ' '
         << (ok ? "(OK)" : "(FAIL)") << endl;

    ret = (ret && ok);

    // d) Try a date that IS NOT in US EASTERN daylight savings
    DateTime test_winter(20050202, 20000000);   // 2 AM

    try
    {
        const DateTime local(20050201,210000000);     // 9PM
        test_winter.toLocalTime();

        ok = ( local == test_winter );
    }
    catch(std::exception&)
    {
        ok = false;
    }

    cout << "  Winter DST Test:" << endl
        << "    " << DateTime::DAY[test_winter.dayOfWeek()] << ' ' << test_winter.day() << ' '
        << DateTime::MONTH[test_winter.month()] << ' ' << setfill('0') << setw(4)
        << test_winter.year()   << ' ' << setw(2) << test_winter.hour() << ':' << setw(2)
        << test_winter.minute() << ':' << setw(2) << test_winter.sec()  << setfill(' ') << ' '
        << (ok ? "(OK)" : "(FAIL)") << endl;

    ret = (ret && ok);

    // e) Try a date that IS in US EASTERN daylight savings
    DateTime test_summer(20050602, 20000000);   // 2 AM

    try
    {
        const DateTime local(20050601,220000000);     // 10 PM
        test_summer.toLocalTime();

        ok = (local == test_summer);
    }
    catch(std::exception&)
    {
        ok = false;
    }

    cout << "  Summer DST test:" << endl
        << "    " << DateTime::DAY[test_summer.dayOfWeek()] << ' ' << test_summer.day() << ' '
        << DateTime::MONTH[test_summer.month()] << ' ' << setfill('0') << setw(4)
        << test_summer.year()   << ' ' << setw(2) << test_summer.hour() << ':' << setw(2)
        << test_summer.minute() << ':' << setw(2) << test_summer.sec()  << setfill(' ') << ' '
        << (ok ? "(OK)" : "(FAIL)") << endl;

    ret = (ret && ok);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
