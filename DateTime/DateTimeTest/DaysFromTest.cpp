#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateFuncs.h"
//#include "mrutil.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using mr::DateFuncs::daysFrom;
using mr::DateTime;

///////////////////////////////////////////////////////////////////////////////

bool
DaysFromTest::basicTest()
{
    struct TestData
    {
        unsigned long inputdate;
        long days;
        unsigned long expected;
        bool expect_fail;
    };

    static const TestData test_data[] = {
        { 20020813,        0, 20020813, false },
        { 20020813,        1, 20020814, false },
        { 20020801,        7, 20020808, false },
        { 20020101,       31, 20020201, false },
        { 20000201,       29, 20000301, false },
        { 20010201,       28, 20010301, false },
        { 20020101,       59, 20020301, false },
        { 20010101,      365, 20020101, false },
        { 19960101,      366, 19970101, false },
        { 14000301,  1460971, 54000302, false },
        { 40040228,        1, 40040229, false },
        { 40040228,        2, 40040301, false },
        { 20020814,       -1, 20020813, false },
        { 20020808,       -7, 20020801, false },
        { 20020201,      -31, 20020101, false },
        { 20000301,      -29, 20000201, false },
        { 20010301,      -28, 20010201, false },
        { 20020301,      -59, 20020101, false },
        { 20020101,     -365, 20010101, false },
        { 19970101,     -366, 19960101, false },
        { 14000302,       -1, 14000301, false },
        { 14000302,       -2, 14000228, false  },
    };

    static const int test_data_size = sizeof(test_data) / sizeof(TestData);

    int i;
    bool ret = true;
	bool ok = false;

    cout << "Basic test:" << endl;

    for (i = 0 ; ret && i < test_data_size ; i++) {
        DateTime input(test_data[i].inputdate, 0);
        DateTime expected(test_data[i].expected, 0);
        DateTime output;

        cout << "  daysFrom(" << test_data[i].days << ", "
                << test_data[i].inputdate << ") = " << flush;

        try {
            output = daysFrom(test_data[i].days, input);
            ok = (!test_data[i].expect_fail) && (output == expected);
            cout << toUInt64(output);
        } catch (std::runtime_error) {
            ok = test_data[i].expect_fail;
            cout << "<failure>";
        }

        if (test_data[i].expect_fail) {
            cout << " (expected failure)";
        } else {
            cout << " (expected " << test_data[i].expected << ')';
        }

		ret &= ok;
        cout << " - " << (ret ? "OK" : "FAIL") << endl;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////

void
DaysFromTest::perfTest()
{
    static const int iterations = 1000000;
    DWORD ticks[4];
    long result;

    cout << "Performance test: " << endl
            << "  " << iterations << " iterations of loop using WhichDay()/"
            "UnWhichDay(): " << flush;

    ticks[0] = GetTickCount();

    for (int i = 0 ; i < iterations ; i++) {
        result = UnWhichDay(WhichDay(19960101) + 366);
    }

    ticks[1] = GetTickCount();

    cout << (ticks[1] - ticks[0]) << "ms" << endl
            << "  " << iterations << " iterations of loop using "
            "daysFrom(): " << flush;

    DateTime from(19960101, 0);
    DateTime to;

    ticks[2] = GetTickCount();

    for (int i = 0 ; i < iterations ; i++) {
        to = daysFrom(366, from);
    }

    ticks[3] = GetTickCount();

    double factor = (double) (ticks[3] - ticks[2]) /
                    (double) (ticks[1] - ticks[0]);

    cout << (ticks[3] - ticks[2]) << "ms" << endl
            << "  = " << factor * 100 << "% of time taken by WhichDay()/"
            "UnWhichDay()" << endl;
}

///////////////////////////////////////////////////////////////////////////////
