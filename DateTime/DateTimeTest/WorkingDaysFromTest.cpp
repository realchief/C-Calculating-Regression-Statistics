#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateFuncs.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using mr::DateFuncs::workingDaysFrom;
using mr::DateTime;

///////////////////////////////////////////////////////////////////////////////

bool
WorkingDaysFromTest::basicTest()
{
    struct TestData
    {
        unsigned long inputdate;
        long days;
        unsigned long expected;
    };

    static const TestData test_data[] = {
        { 20020805,   0, 20020805 },
        { 20020805,   1, 20020806 },
        { 20020802,   1, 20020805 },
        { 20020805,   4, 20020809 },
        { 20020802,   6, 20020812 },
        { 20020804,   6, 20020812 },
        { 20020804,   7, 20020813 },
        { 20020802,   7, 20020813 },
        { 20020802,  10, 20020816 },
        { 20020101,  22, 20020131 },
        { 20020806,  -1, 20020805 },
        { 20020805,  -1, 20020802 },
        { 20020809,  -4, 20020805 },
        { 20020812,  -6, 20020802 },
        { 20020813,  -6, 20020805 },
        { 20020813,  -7, 20020802 },
        { 20020816, -10, 20020802 },
        { 20020131, -22, 20020101 },
    };

    static const int test_data_size = sizeof(test_data) / sizeof(TestData);

    int i;
    bool ret = true;

    cout << "Basic test:" << endl;

    for (i = 0 ; ret && i < test_data_size ; i++) {
        DateTime input(test_data[i].inputdate, 0);
        DateTime expected(test_data[i].expected, 0);

        cout << "  workingDaysFrom(" << test_data[i].days << ", "
                << test_data[i].inputdate << ") = " << flush;

        DateTime result = workingDaysFrom(test_data[i].days, input);
        ret = (result == expected);

        cout << result.getDate() << " (expected " << test_data[i].expected
                << ") - " << (ret ? "OK" : "FAIL") << endl;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////

void
WorkingDaysFromTest::perfTest()
{
    static const int iterations = 1000000;
    DWORD ticks[4];
    long result;

    cout << "Performance test: " << endl
            << "  " << iterations << " iterations of loop using WhichDay()/"
            "UnWhichDay(): " << flush;

    ticks[0] = GetTickCount();

    for (int i = 0 ; i < iterations ; i++) {
        result = UnWhichDay(WhichDay(20020802, true) + 4, true);
    }

    ticks[1] = GetTickCount();

    cout << (ticks[1] - ticks[0]) << "ms" << endl
            << "  " << iterations << " iterations of loop using "
            "workingDaysFrom(): " << flush;

    DateTime from(20020802, 0);
    DateTime to;

    ticks[2] = GetTickCount();

    for (int i = 0 ; i < iterations ; i++) {
        to = workingDaysFrom(4, from);
    }

    ticks[3] = GetTickCount();

    double factor = (double) (ticks[3] - ticks[2]) /
                    (double) (ticks[1] - ticks[0]);

    cout << (ticks[3] - ticks[2]) << "ms" << endl
            << "  = " << factor * 100 << "% of time taken by WhichDay()/"
            "UnWhichDay()" << endl;
}

///////////////////////////////////////////////////////////////////////////////
