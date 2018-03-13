#include "stdafx.h"
#include "DateTimeTest.h"
#include "DateTime/MRDateTime.h"
//#include "mrutil.h"

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

///////////////////////////////////////////////////////////////////////////////

/*****  Static member initialisers  *****/

vector<DateTimeTestBase *> DateTimeTestBase::_tests;

///////////////////////////////////////////////////////////////////////////////

/*****  Globals  *****/

// The tests
DateTimeTest1 test1;
DateTimeTest2 test2;
DateTimeTest3 test3;
DateTimeTest4 test4;
DateTimeTest5 test5;
DateTimeTest6 test6;
DateTimeTest7 test7;
DateTimeTest8 test8;
DateTimeTest9 test9;
DateTimeTest10 test10;
DateTimeSpanTest1 test11;
DateTimeSpanTest2 test12;
DateTimeSpanTest3 test13;
DateTimeSpanTest4 test14;
DateTimeSpanTest5 test15;
DateTimeSpanTest6 test16;
DateTimeTest11 test17;
DaysBetweenTest test18;
DaysFromTest test19;
WorkingDaysBetweenTest test20;
WorkingDaysFromTest test21;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

DateTimeTestBase::DateTimeTestBase(const char *description)
    : _descr(description)
{
    _tests.push_back(this);
}

///////////////////////////////////////////////////////////////////////////////

DateTimeTestBase::~DateTimeTestBase()
{
}

///////////////////////////////////////////////////////////////////////////////

bool
DateTimeTestBase::runTests()
{
    unsigned int failed = 0;

    for (vector<DateTimeTestBase *>::iterator i = _tests.begin(); 
		i != _tests.end() ; ++i)
	{
        cout << endl << (*i)->_descr << ':' << endl << endl;

        if ((*i)->test()) 
		{
            cout << endl << "----------------  PASS  ----------------" << endl;
        }
		else 
		{
			++failed;
            cout << endl << "----------------  FAIL  ----------------" << endl;
        }
    }

	cout << endl << "================  " << _tests.size() << " test(s) total; "
         << failed << " failed  ================" << endl;

    return failed == 0;
}

///////////////////////////////////////////////////////////////////////////////

bool
DateFuncTest::test()
{
    bool ret = basicTest();
    cout << endl;
    perfTest();
    return ret;
}

///////////////////////////////////////////////////////////////////////////////

mr::uint64
toUInt64(const mr::DateTime& dt)
{
    mr::uint64 result = dt.getDate();
    result *= 1000000000;
    result += dt.getTime();

    return result;
}

///////////////////////////////////////////////////////////////////////////////

int
main(int /*argc*/, char *argv[])
{
	int ret = 0;

	// Initialize MFC and print and error on failure
	//if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	//{
	//	cerr << argv[0] << ": fatal error: MFC initialisation failed" << endl;
	//	ret = 1;
	//}
	//else
	{
	    if (!DateTimeTestBase::runTests()) {
	        ret = 2;
	    }
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
