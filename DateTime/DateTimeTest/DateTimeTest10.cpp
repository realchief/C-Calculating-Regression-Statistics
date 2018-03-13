// 10. DateTime toMillis()/fromMillis() functions
//     a) Construct DateTime object with 00:00:00.000 1st Mar 0004
//     b) Construct DateTime object with 00:01:00.000 1st Mar 0004
//     c) Construct DateTime object with 23:59:59.999 29th Feb 4004
//     d) Construct DateTime object with 00:00:00.000 1st Jan 1970
//     e) Call toMillis() for each of the above objects and store results;
//        these must be:
//          i) 0
//         ii) 60000
//        iii) 126227980799999
//         iv) 62035977600000
//     f) Construct four new DateTime objects and initialise them with the
//        above millisecond values using fromMillis(), these should be equal
//        to those constructed earlier
//     g) Attempt to initialise a DateTime object using fromMillis() with
//        value 126227980800000 - this must fail

#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

using namespace boost;
using namespace boost::posix_time;
using namespace std;

using mr::DateTime;
using mr::DateTimeSpan;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

bool
DateTimeTest10::test()
{
    bool ret = true;
    bool ok;

    // c) Construct DateTime object with 23:59:59.999 29th Feb 4004
    // d) Construct DateTime object with 00:00:00.000 1st Jan 1970
    DateTime c(40040229, 235959999);
    DateTime d(DateTime::DT_TM_EPOCH);
    unsigned __int64 ms[4];

    cout << "                      c (29 Feb 4004  23:59:59.999)" << endl
         << "                      d (01 Jan 1970  00:00:00:000)" << endl
         << endl;

    // e) Call toMillis() for each of the above objects and store results;
    //    these must be:
    //      i) 0
    //     ii) 60000
    //    iii) 12622780799999
    //     iv) 62035804800000
    cout << "  DateTime::toMillis() test:" << endl;

    ms[2] = c.toMillis();
    ok = (ms[2] == 64192003199999);
    cout << "    c.toMillis() = " << ms[2] << " (expected 64192003199999): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    ms[3] = d.toMillis();
    ok = (ms[3] == 0);
    cout << "    d.toMillis() = " << ms[3] << " (expected 0): "
         << (ok ? "OK" : "FAIL") << endl;
    ret = (ret && ok);

    // f) Construct four new DateTime objects and initialise them with the
    //    above millisecond values using fromMillis(), these should be equal
    //    to those constructed earlier
    DateTime g;
    DateTime h;

    cout << endl << "  DateTime::fromMillis() test (with 5 new DateTime "
            "objects - e, f, g, h and i):" << endl;


    try {
		g = DateTime::fromMillis(ms[2]);

		time_duration diff = (g.asPtime() - c.asPtime());

// 		int64_t diff_ticks = diff.ticks();

        ok = (g == c);
        cout << "    After g.fromMillis(" << ms[2] << "), result of (g == c)"
                " = " << (ok ? "true" : "false") << " (expected true): "
             << (ok ? "OK" : "FAIL") << endl;
        ret = (ret && ok);
    } catch (std::runtime_error) {
        cout << "    g.fromMillis(" << ms[2] << ") threw exception: FAIL"
             << endl;
    }

    try {
        h = DateTime::fromMillis(ms[3]);

		time_duration diff = (h.asPtime() - d.asPtime());

		int64_t diff_ticks = diff.ticks();

        ok = (h == d);
        cout << "    After h.fromMillis(" << ms[3] << "), result of (h == d)"
                " = " << (ok ? "true" : "false") << " (expected true): "
             << (ok ? "OK" : "FAIL") << endl;
        ret = (ret && ok);
    } catch (std::runtime_error) {
        cout << "    h.fromMillis(" << ms[3] << ") threw exception: FAIL"
             << endl;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
