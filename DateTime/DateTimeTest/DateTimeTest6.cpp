// 6. daysInMonth()
//     a) Use static daysInMonth() to return days in all months of 2000
//     b) Use non-static single-parameter daysInMonth() to return days in all
//        months for DateTime object constructed for year 2035
//     c) Use non-static parameterless daysInMonth() on DateTime object set to
//        1st Jan 1970

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
DateTimeTest6::test()
{
    static const size_t expected[] = {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,   // mths of year 2000
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,   // mths of year 2035
    };

    bool ret = true;
    bool ok;
    int i;
    int j;

    // a) Use static daysInMonth() to return days in all months of 2000
    cout << "  Static DateTime::daysInMonth() function:" << endl;

    for (i = 1, j = 0 ; i <= 12 ; i++, j++) {
        ok = (DateTime::daysInMonth(i, 2000) == expected[j]);
        cout << "    DateTime::daysInMonth(" << i << ", 2000) = "
             << (long) DateTime::daysInMonth(i, 2000) << " (expected "
             << (long) expected[j] << "): " << (ok ? "OK" : "FAIL") << endl;
        ret = (ret && ok);
    }

    // b) Use non-static single-parameter daysInMonth() to return days in all
    //    months for DateTime object constructed for year 2035
    cout << endl << "  daysInMonth() member function for DateTime object set "
            "to year 2035:" << endl;

    DateTime dt(20350101, 0);

    for (i = 1 ; i <= 12 ; i++, j++) {
        ok = (dt.daysInMonth(i) == expected[j]);
        cout << "    daysInMonth(" << i << ") = " << (long) dt.daysInMonth(i)
             << " (expected " << (long) expected[j] << "): "
             << (ok ? "OK" : "FAIL") << endl;
        ret = (ret && ok);
    }

    // c) Use non-static parameterless daysInMonth() on DateTime object
    //    set to 1st Jan 1970
    cout << endl << "  daysInMonth() member function for DateTime object set "
            "to 1st Jan 1970" << endl;

    dt.setDate(19700101);

    ok = (dt.daysInMonth() == 31);
    cout << "    daysInMonth() = " << (long) dt.daysInMonth()
         << " (expected 31): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
