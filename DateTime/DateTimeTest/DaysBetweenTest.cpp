#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateFuncs.h"
//#include "mrutil.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using mr::DateFuncs::daysBetween;
using mr::DateTime;

///////////////////////////////////////////////////////////////////////////////

bool
DaysBetweenTest::basicTest()
{
    struct TestData
    {
        unsigned long fromdate;
        unsigned long fromtime;
        unsigned long todate;
        unsigned long totime;
        long expected;
    };

    static const TestData test_data[] = {
        { 20020813, 000000000, 20020813, 000000000, 0 },
        { 20020813, 000000000, 20020814, 000000000, 1 },
        //{ 20020813, 235959999, 20020814, 000000000, 1 },
        { 20020801, 000000000, 20020808, 000000000, 7 },
        { 20020101, 000000000, 20020201, 000000000, 31 },
        { 20000201, 000000000, 20000301, 000000000, 29 },
        { 20010201, 000000000, 20010301, 000000000, 28 },
        { 20020101, 000000000, 20020301, 000000000, 59 },
        { 20010101, 000000000, 20020101, 000000000, 365 },
        { 19960101, 000000000, 19970101, 000000000, 366 },
        { 14000301, 000000000, 40040229, 000000000, 951091 },
        { 20020814, 000000000, 20020813, 000000000, -1 },
        //{ 20020814, 000000000, 20020813, 235959999, -1 },
        { 20020808, 000000000, 20020801, 000000000, -7 },
        { 20020201, 000000000, 20020101, 000000000, -31 },
        { 20000301, 000000000, 20000201, 000000000, -29 },
        { 20010301, 000000000, 20010201, 000000000, -28 },
        { 20020301, 000000000, 20020101, 000000000, -59 },
        { 20020101, 000000000, 20010101, 000000000, -365 },
        { 19970101, 000000000, 19960101, 000000000, -366 },
        { 40040229, 000000000, 14000301, 000000000, -951091 },
    };

    static const int test_data_size = sizeof(test_data) / sizeof(TestData);

    int i;
    bool ret = true;

    cout << "Basic test:" << endl;

    for (i = 0 ; ret && i < test_data_size ; i++) {
        DateTime from(test_data[i].fromdate, test_data[i].fromtime);
        DateTime to(test_data[i].todate, test_data[i].totime);

        cout << "  daysBetween(" << toUInt64(from) << ", " << toUInt64(to)
            << ") = " << flush;

        long result = daysBetween(from, to);

        ret = (result == test_data[i].expected);

        cout << result << " (expected " << test_data[i].expected << ") - "
                << (ret ? "OK" : "FAIL") << endl;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////

void
DaysBetweenTest::perfTest()
{
    static const int iterations = 10000000;
    long diff = 0;
    DWORD ticks[4];

	std::vector<int> dates1(iterations);
	std::vector<DateTime> dates2(iterations);

	DateTime from(19960101, 0);
    DateTime to(19970101, 0);

	for (int i = 0; i < iterations; ++i)
	{
		dates1[i] = (i % 2) ? 19960101 : 19970101;
		dates2[i] = (i % 2) ? from : to;
	}

    cout << "Performance test: " << endl
            << "  " << iterations << " iterations of loop using WhichDay(): "
            << flush;

    ticks[0] = GetTickCount();

    for (int i = 1 ; i < iterations ; i+=2) {
        diff += WhichDay(dates1[i-1]) - WhichDay(dates1[i]);
    }

    ticks[1] = GetTickCount();

    cout << (ticks[1] - ticks[0]) << "ms" << endl
			<< "calculated diff: " << diff << endl
            << "  " << iterations << " iterations of loop using "
            "daysBetween(): "  << flush;

    ticks[2] = GetTickCount();

	diff = 0;
    for (int i = 1; i < iterations ; i+=2) {
		diff += (dates2[i-1].asPtime() - dates2[i].asPtime()).hours() / 24;
    }

    ticks[3] = GetTickCount();

    double factor = (double) (ticks[3] - ticks[2]) /
                    (double) (ticks[1] - ticks[0]);

    cout << (ticks[3] - ticks[2]) << "ms" << endl
			<< "calculated diff: " << diff << endl
            << "  = " << factor * 100 << "% of time taken by WhichDay()"
            << endl;
}

///////////////////////////////////////////////////////////////////////////////
