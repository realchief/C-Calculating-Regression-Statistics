// 13. DateTimeSpan setter functions
//     a) Construct DateTimeSpan object with default constructor
//     b) Call two-parameter set() function with DateTime objects with values
//        00:00:00.000 1st Mar 0004 and 23:59:59.999 29th Feb 4004
//        (value must be 126227980799999)
//     c) Call two-parameter set() function with DateTime objects with values
//        23:59:59.999 29th Feb 4004 and 00:00:00.000 1st Mar 0004
//        (value must be -126227980799999)
//     d) Call five-parameter set() function with value 10 for 'days', 12 for
//        'hrs', 30 for 'mins', 59 for 'secs' and 765 for 'millis'
//        (value must be 909059765)
//     e) Call five-parameter set() function with value 10 for 'days',
//        -12 for 'hrs', 30 for 'mins', -59 for 'secs' and 765 for 'millis'
//        (value must be 822541765)
//     f) Call five-parameter set() function with value -10 for 'days',
//        -12 for 'hrs', -30 for 'mins', -59 for 'secs' and -765 for 'millis'
//        (value must be -909059765)

#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using namespace std;
using namespace boost;

using mr::DateTime;
using mr::DateTimeSpan;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeSpanTest3::test()
{
    bool ret = true;
    bool ok;

    // a) Construct DateTimeSpan object with default constructor
    DateTimeSpan a;

    cout << "  For a single DateTimeSpan object, 'a':" << endl;

    // b) Call two-parameter set() function with DateTime objects with values
    //    00:00:00.000 1st Mar 1400 and 23:59:59.999 29th Feb 4004
    //    (value must be 82174348799999)
    a.set(DateTime(14000301, 0), DateTime(40040229, 235959999));

    ok = (a.totalMillis() == 82174348799999);
    cout << "    After a.set(DateTime(14000301, 0), DateTime(40040229, "
            "235959999))," << endl << "      value of a = " << a.totalMillis()
         << " (expected 82174348799999): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    // c) Call two-parameter set() function with DateTime objects with values
    //    23:59:59.999 29th Feb 4004 and 00:00:00.000 1st Mar 1400
    //    (value must be -82174348799000)
    a.set(DateTime(40040229, 235959999), DateTime(14000301, 0));
    
	ok = (a.totalMillis() == -82174348799999);
    cout << "    After a.set(DateTime(40040229, 235959999), "
            "DateTime(14000301, 0))," << endl << "      value of a = "
         << a.totalMillis() << " (expected -82174348799999): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    // d) Call five-parameter set() function with value 10 for 'days', 12 for
    //    'hrs', 30 for 'mins', 59 for 'secs' and 765 for 'millis'
    //    (value must be 909059765)
    a.set(10, 12, 30, 59, 765);
    ok = (a.totalMillis() == 909059765);
    cout << "    After a.set(10, 12, 30, 59, 765)," << endl
         << "      value of a = " << a.totalMillis()
         << " (expected 909059765): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    // e) Call five-parameter set() function with value 10 for 'days',
    //    -12 for 'hrs', 30 for 'mins', -59 for 'secs' and 765 for 'millis'
    //    (value must be 822541765)
    a.set(10, -12, 30, -59, 765);
    ok = (a.totalMillis() == 822541765);
    cout << "    After a.set(10, -12, 30, -59, 765)," << endl
         << "      value of a = " << a.totalMillis()
         << " (expected 822541765): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    // f) Call five-parameter set() function with value -10 for 'days',
    //    -12 for 'hrs', -30 for 'mins', -59 for 'secs' and -765 for 'millis'
    //    (value must be -909059765)
    a.set(-10, -12, -30, -59, -765);
    ok = (a.totalMillis() == -909059765);
    cout << "    After a.set(-10, -12, -30, -59, -765)," << endl
         << "      value of a = " << a.totalMillis()
         << " (expected -909059765): " << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
