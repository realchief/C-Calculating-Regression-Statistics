#ifndef __DATETIMETEST_H__
#define __DATETIMETEST_H__

#include <vector>

#include "MRIntTypes.h"

///////////////////////////////////////////////////////////////////////////////

/*****  Forward Declarations  *****/

namespace mr
{
    class DateTime;
};

///////////////////////////////////////////////////////////////////////////////

/*****  Data Structures, Types and Classes  *****/

class DateTimeTestBase
{
public:
    DateTimeTestBase(const char *description);
    virtual ~DateTimeTestBase();

    virtual bool test() = 0;

    static bool runTests();

private:
    const char *_descr;

    static std::vector<DateTimeTestBase *> _tests;
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest1 : public DateTimeTestBase
{
public:
    inline DateTimeTest1() :
        DateTimeTestBase("mr::DateTime construction") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest2 : public DateTimeTestBase
{
public:
    inline DateTimeTest2() :
        DateTimeTestBase("mr::DateTime getter functions") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest3 : public DateTimeTestBase
{
public:
    inline DateTimeTest3() :
        DateTimeTestBase("mr::DateTime setter functions") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest4 : public DateTimeTestBase
{
public:
    inline DateTimeTest4() :
        DateTimeTestBase("mr::DateTime's now()/LocalTime()/GMT() "
                         "functions") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest5 : public DateTimeTestBase
{
public:
    inline DateTimeTest5() :
        DateTimeTestBase("mr::DateTime::isLeapYear()") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest6 : public DateTimeTestBase
{
public:
    inline DateTimeTest6() :
        DateTimeTestBase("mr::DateTime::daysInMonth()") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest7 : public DateTimeTestBase
{
public:
    inline DateTimeTest7() :
        DateTimeTestBase("mr::DateTime conversion operators") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest8 : public DateTimeTestBase
{
public:
    inline DateTimeTest8() :
        DateTimeTestBase("mr::DateTime comparison operators") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest9 : public DateTimeTestBase
{
public:
    inline DateTimeTest9() :
        DateTimeTestBase("mr::DateTime assignment operators") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest10 : public DateTimeTestBase
{
public:
    inline DateTimeTest10() :
        DateTimeTestBase("mr::DateTime's toMillis()/fromMillis() functions") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeTest11 : public DateTimeTestBase
{
public:
    inline DateTimeTest11() :
        DateTimeTestBase("mr::DateTime arithmetic operators") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeSpanTest1 : public DateTimeTestBase
{
public:
    inline DateTimeSpanTest1() :
        DateTimeTestBase("mr::DateTimeSpan construction and conversion "
                         "operator") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeSpanTest2 : public DateTimeTestBase
{
public:
    inline DateTimeSpanTest2() :
        DateTimeTestBase("mr::DateTimeSpan getter functions") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeSpanTest3 : public DateTimeTestBase
{
public:
    inline DateTimeSpanTest3() :
        DateTimeTestBase("mr::DateTimeSpan setter functions") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeSpanTest4 : public DateTimeTestBase
{
public:
    inline DateTimeSpanTest4() :
        DateTimeTestBase("mr::DateTimeSpan comparison operators") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeSpanTest5 : public DateTimeTestBase
{
public:
    inline DateTimeSpanTest5() :
        DateTimeTestBase("mr::DateTimeSpan assignment operator") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateTimeSpanTest6 : public DateTimeTestBase
{
public:
    inline DateTimeSpanTest6() :
        DateTimeTestBase("mr::DateTimeSpan arithmetic operators") {}

    virtual bool test();
};

///////////////////////////////////////////////////////////////////////////////

class DateFuncTest : public DateTimeTestBase
{
public:
    inline DateFuncTest(const char *description) :
        DateTimeTestBase(description) {}

    virtual bool test();

private:
    virtual bool basicTest() = 0;
    virtual void perfTest() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class DaysBetweenTest : public DateFuncTest
{
public:
    inline DaysBetweenTest() : DateFuncTest("mr::DateFuncs::daysBetween()") {}

private:
    virtual bool basicTest();
    virtual void perfTest();
};

///////////////////////////////////////////////////////////////////////////////

class DaysFromTest : public DateFuncTest
{
public:
    inline DaysFromTest() : DateFuncTest("mr::DateFuncs::daysFrom()") {}

private:
    virtual bool basicTest();
    virtual void perfTest();
};

///////////////////////////////////////////////////////////////////////////////

class WorkingDaysBetweenTest : public DateFuncTest
{
public:
    inline WorkingDaysBetweenTest() :
        DateFuncTest("mr::DateFuncs::workingDaysBetween()") {}

private:
    virtual bool basicTest();
    virtual void perfTest();
};

///////////////////////////////////////////////////////////////////////////////

class WorkingDaysFromTest : public DateFuncTest
{
public:
    inline WorkingDaysFromTest() :
        DateFuncTest("mr::DateFuncs::workingDaysFrom()") {}

private:
    virtual bool basicTest();
    virtual void perfTest();
};

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

mr::uint64 toUInt64(const mr::DateTime& dt);

///////////////////////////////////////////////////////////////////////////////

#endif /* __DATETIMETEST_H__ */
