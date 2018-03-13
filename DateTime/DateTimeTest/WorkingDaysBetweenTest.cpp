#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"
#include "DateTime/MRDateFuncs.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using mr::DateFuncs::workingDaysBetween;
using mr::DateTime;
using mr::DateTimeSpan;

///////////////////////////////////////////////////////////////////////////////

bool
WorkingDaysBetweenTest::basicTest()
{
    struct TestData
    {
        unsigned long fromdate;
        unsigned long todate;
        long expected;
    };

    static const TestData test_data[] = {
        { 20020805, 20020805, 0 },
        { 20020805, 20020806, 1 },
        { 20020805, 20020809, 4 },
        { 20020805, 20020810, 4 },
        { 20020805, 20020811, 4 },
        { 20020804, 20020811, 5 },
        { 20020804, 20020812, 6 },
        { 20020804, 20020813, 7 },
        { 20020802, 20020813, 7 },
        { 20020802, 20020816, 10 },
        { 20020101, 20020131, 22 },
        { 20020806, 20020805, -1 },
        { 20020809, 20020805, -4 },
        { 20020810, 20020805, -4 },
        { 20020811, 20020805, -4 },
        { 20020811, 20020804, -5 },
        { 20020812, 20020804, -6 },
        { 20020813, 20020804, -7 },
        { 20020813, 20020802, -7 },
        { 20020816, 20020802, -10 },
        { 20020131, 20020101, -22 },
		{ 20100109, 20100102, -5 }
    };

    static const int test_data_size = sizeof(test_data) / sizeof(TestData);

    int i;
    bool ret = true;

    cout << "Basic test:" << endl;

    for (i = 0 ; /*ret &&*/ i < test_data_size ; i++) {
        DateTime from(test_data[i].fromdate, 0);
        DateTime to(test_data[i].todate, 0);

        cout << "  workingDaysBetween(" << test_data[i].fromdate << ", "
                << test_data[i].todate << ") = " << flush;

        long result = workingDaysBetween(from, to);

        ret = (result == test_data[i].expected);

        cout << result << " (expected " << test_data[i].expected << ") - "
                << (ret ? "OK" : "FAIL") << endl;
    }

	
	/*std::cout << "Testing Which day - DateTime versions of WorkingDaysBetween are equal" << std::endl;

	DateTime test_begin_dt(1970, 1, 1);
	const DateTime test_end_dt(2050, 1, 1);
	const DateTimeSpan ONE_DAY(1, 0, 0, 0, 0);

	for (DateTime dt1 = test_begin_dt; dt1 < test_end_dt && ret; dt1 += ONE_DAY)
	{
		std::cout << dt1.getDate() << std::endl;
		for (DateTime dt2 = test_begin_dt; dt2 < test_end_dt; dt2 += ONE_DAY)
		{
			ret = (workingDaysBetween(dt1, dt2) == workingDaysBetween(dt1.getDate(), dt2.getDate()));
			if (!ret)
			{
				std::cout << "Detected difference at " << dt1.getDate() << ' ' << dt2.getDate() << std::endl
						  << "DateTimeBased = " << workingDaysBetween(dt1, dt2) << std::endl
						  << "WhichDayBased = " << workingDaysBetween(dt1.getDate(), dt2.getDate()) << std::endl;
				break;
			}
		}
	}
	if (ret)
	{
		std::cout << "Test OK" << std::endl;
	}*/


    return ret;
}

///////////////////////////////////////////////////////////////////////////////

void
WorkingDaysBetweenTest::perfTest()
{
    static const int iterations = 10000000;
    long diff = 0;
    DWORD ticks[4];

	std::vector<int> dates1(iterations);
	std::vector<DateTime> dates2(iterations);



	for (int i = 0; i < iterations; ++i)
	{
		dates2[i] = DateTime(DateTime().asPtime() + boost::gregorian::date_duration(rand() % 1000));
		dates1[i] =  dates2[i].getDate();
	}

    cout << "Performance test: " << endl
            << "  " << iterations << " iterations of loop using WhichDay(): "
            << flush;

	ticks[2] = GetTickCount();

	for (int i = 1 ; i < iterations ; ++i) {
		diff += workingDaysBetween(dates2[12], dates2[13]);
    }

    ticks[3] = GetTickCount();

	diff = 0;

    ticks[0] = GetTickCount();

    for (int i = 1 ; i < iterations ; ++i) {
        diff += workingDaysBetween(dates1[12], dates1[13]);
    }

    ticks[1] = GetTickCount();

    cout << (ticks[1] - ticks[0]) << "ms" << endl
		<< "diff: " << diff << endl
            << "  " << iterations << " iterations of loop using "
            "workingDaysBetween(): " << flush;

   

    double factor = (double) (ticks[3] - ticks[2]) /
                    (double) (ticks[1] - ticks[0]);

    cout << (ticks[3] - ticks[2]) << "ms" << endl
		<< "diff: " << diff << endl
            << "  = " << factor * 100 << "% of time taken by WhichDay()"
            << endl;
}

///////////////////////////////////////////////////////////////////////////////
