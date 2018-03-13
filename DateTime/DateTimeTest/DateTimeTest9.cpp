// 9. DateTime assignment operators
//    a) Create time_t with value 86399
//    b) Construct COleDateTime object with 23:59:59 1st Jan 1970
//    c) Construct COleDateTime object with 23:59:59 29th Feb 4004
//    d) Construct COleDateTime object with 00:00:00 1st Mar 4004
//    e) Construct CTime object with 23:59:59 1st Jan 1970
//    f) Construct five DateTime objects
//    g) Assign time_t value to first DateTime object - this must succeed
//    h) Assign first COleDateTime object to second DateTime object - this
//       must succeed
//    i) Assign second COleDateTime object to third DateTime object - this
//       must succeed
//    j) Assign third COleDateTime object to fourth DateTime object - this
//       must fail (DateTime object must be unchanged with value
//       00:00:00.000 1st Jan 1970)
//    k) Assign CTime object to fifth DateTime object - this must succeed

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
DateTimeTest9::test()
{
    bool ret = true;
    bool ok;

    time_t foreign1 = 86399;
    DateTime a;
    DateTime b;
    DateTime c;
    DateTime d;
    DateTime e;

    try {
        cout << "  DateTime a = (time_t) 86399" << endl << "  (result = ";
		a = DateTime::fromTime_t(foreign1);
        cout << toUInt64(a);
        ok = (toUInt64(a) == 19700101235959000);
    } catch (std::runtime_error) {
        cout << "<failed>";
        ok = false;
    }

    cout << ", expected 19700101235959000): " << (ok ? "OK" : "FAIL")
         << endl << endl;
    ret = (ret && ok);


    return ret;
}

///////////////////////////////////////////////////////////////////////////////
