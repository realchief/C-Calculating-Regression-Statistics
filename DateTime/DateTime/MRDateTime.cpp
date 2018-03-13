#include "Common.h"
#include "DateTime/DateTimeAPI.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"
#include "Infrastructure/MathUtils.h"

#include "DesignByContract.h"
#include "DebugNew.h"

namespace mr
{
	using namespace boost;
	using namespace boost::gregorian;
	using boost::date_time::dynamic_local_time_adjustor;
	using boost::date_time::c_local_adjustor;
	typedef boost::gregorian::gregorian_calendar::year_type year_type;
	typedef boost::gregorian::gregorian_calendar::month_type month_type;
	typedef boost::gregorian::gregorian_calendar::day_type day_type;
	typedef boost::posix_time::time_duration::hour_type hour_type;
	typedef boost::posix_time::time_duration::min_type min_type;
	typedef boost::posix_time::time_duration::sec_type sec_type;
	typedef boost::posix_time::time_duration::fractional_seconds_type fractional_seconds_type;
	using namespace boost::posix_time;
	using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*****  Data Types, Structures & Classes  *****/

typedef boost::date_time::local_adjustor<ptime, -5, us_dst> us_eastern;

///////////////////////////////////////////////////////////////////////////////

/*****  Static member initialisers  *****/

const char * const mr::DateTime::DAY[] = {
    "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
    "Friday"
};

const char * const mr::DateTime::DAY_ABBREV[] = {
    "Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"
};

const char * const mr::DateTime::MONTH[] = {
    "", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

const char * const mr::DateTime::MONTH_ABBREV[] = {
    "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const char * const mr::DateTime::SEASON[] = {
    "", "Winter", "Spring", "Summer", "Autumn"
};

const char * const mr::DateTime::QUARTER[] = {
    "", "Q1", "Q2", "Q3", "Q4"
};

const ptime DateTime::DT_TM_EPOCH(gregorian::date(1970,01,01));
const ptime DateTime::DT_TM_NULL(gregorian::not_a_date_time);

const DateTime DateTime::Null (DateTime::DT_TM_NULL);
const DateTime DateTime::Epoch(DateTime::DT_TM_EPOCH);

const double DateTime::NaN(NAN_INVALID);

BOOL SystemTimeToVariantTime000(/*input*/ SYSTEMTIME st, /*output*/double *dVariantTime);
BOOL VariantTimeToSystemTime000(/*input*/ double dVariantTime, /*output*/SYSTEMTIME *st);


class NestingCount
{
public:
	NestingCount(int& count)
		: _count(count)
	{
		++_count;
	}

	~NestingCount()
	{
		--_count;
	}

	bool isNested() const
	{
		return _count > 1;
	}

private:
	void operator=(const NestingCount&);
	int& _count;
};

///////////////////////////////////////////////////////////////////////////////
static const unsigned __int64 MILLI_SEC_PER_DAY = 1000 * 60 * 60 * 24;
/*****  Functions  *****/

DateTime::DateTime(const boost::gregorian::date& dt)
:	ptime(dt)
{
}

DateTime::DateTime(const boost::posix_time::ptime& date_and_time)
: ptime(date_and_time)
{
}

DateTime::DateTime(unsigned __int64 millis)
:   ptime(DT_TM_EPOCH)
{
	const long days_count = boost::numeric_cast<long>(millis / MILLI_SEC_PER_DAY);
	asPtime() += boost::gregorian::date_duration(days_count);
	asPtime() += milliseconds(boost::numeric_cast<long>(millis % MILLI_SEC_PER_DAY));
}


double
DateTime::toDatasetDouble() const
{
	double result(NaN);

#if _MSC_VER >= 1400
	typedef __time32_t TimeType;
#else
	typedef time_t TimeType;
#endif
    BOOST_STATIC_ASSERT ( sizeof ( uint32_t ) == sizeof ( TimeType ) );

	if(!is_not_a_date_time())
	{
		uint32_t* magic = reinterpret_cast<uint32_t*> ( &result );

		*magic = 0;

		++magic;

		reinterpret_cast<TimeType&>(*magic) = static_cast<TimeType>(toTime_t());
	}

    return result;
}

/*static*/
DateTime
DateTime::fromMillis(unsigned __int64 millis)
{
	const long days_count = boost::numeric_cast<long>(millis / MILLI_SEC_PER_DAY);
	DateTime ret(DT_TM_EPOCH);
	ret.asPtime() += boost::gregorian::date_duration(days_count);
	ret.asPtime() += milliseconds(boost::numeric_cast<long>(millis % MILLI_SEC_PER_DAY));

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

///
/// Construct a @c DateTime instance from discrete date/time components.
/// @param yr  Year, in the range [0004, 4004].
/// @param mth Month, in the range [1, 12].
/// @param dy  Day of the month, in the range [1, (last day of @c mth)].
/// @param h   Hour, in the range [0, 23].
/// @param m   Minute, in the range [0, 59].
/// @param s   Second, in the range [0, 59].
/// @param ms  Millisecond, in the range [0, 999].
/// @pre <tt>yr, mth, dy, h, m, s</tt> and <tt>ms</tt> must be in their
///      appropriate ranges.
/// @pre Date and time must not be earlier than midnight, 1st Jan 4
/// @pre Date and time must be earlier than midnight, 1st Jan 4
///
DateTime::DateTime( unsigned __int32 yr,
                    unsigned __int32 mth,
                    unsigned __int32 dy,
                    unsigned __int32 h,
                    unsigned __int32 m,
                    unsigned __int32 s,
                    unsigned __int32 ms)
:   ptime(DT_TM_EPOCH)
{
    setDate(yr, mth, dy);
    setTime(h, m, s, ms);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Construct a @c DateTime instance using the internal representation format.
/// @param date Date portion in the format @c YYYYMMDD, where
///     - <tt>YYYY</tt> = year (0004 to 4004)
///     - <tt>  MM</tt> = month (1 to 12)
///     - <tt>  DD</tt> = day (1 to 31)
/// 
/// @param time Time portion in the format @c HHMMSSmmm, where
///     - <tt>  HH</tt> = hour (00 to 23)
///     - <tt>  MM</tt> = minute (00 to 59)
///     - <tt>  SS</tt> = second (00 to 59)
///     - <tt> mmm</tt> = millisecond (000 to 999)
/// 
DateTime::DateTime( unsigned long date,
                    unsigned long time)
:   ptime(DT_TM_EPOCH)
{
	setDate(date);
	setTime(time);
}

DateTime::DateTime(boost::posix_time::special_values special_date_time_value)
	: ptime(special_date_time_value)
{
	DBC_REQUIRE(special_date_time_value == boost::posix_time::not_a_date_time);
}


///////////////////////////////////////////////////////////////////////////////
DateTime::DateTime(double raw_dt_tm)
:   ptime(DT_TM_NULL)
{
    if( _finite(raw_dt_tm) )
    {
        SYSTEMTIME st;

        if(VariantTimeToSystemTime(raw_dt_tm, &st))
        {
            setDate(st.wYear, st.wMonth, st.wDay);
            setTime(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// Convenience function to get the stored year, month and day in one call.
/// @param yr  Gets populated with the stored year (0004 to 4004)
/// @param mth Gets populated with the stored month (1 to 12)
/// @param dy  Gets populated with the stored day (1 to 31)
///
void
DateTime::getDate(unsigned __int32 &yr,
                  unsigned __int32 &mth,
                  unsigned __int32 &dy) const
{
	DBC_REQUIRE(!is_not_a_date_time());

	const gregorian::date::ymd_type ymd = date().year_month_day();

	yr = ymd.year;
    mth = ymd.month;
    dy = ymd.day;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Convenience function to get the stored hour, minute second and millisecond
/// in one call.
/// @param h  Gets populated with the stored hour (0 to 23)
/// @param m  Gets populated with the stored minute (0 to 59)
/// @param s  Gets populated with the stored second (0 to 59)
/// @param ms Gets populated with the stored millisecond (0 to 999)
///
void
DateTime::getTime(unsigned __int32 &h,
                  unsigned __int32 &m,
                  unsigned __int32 &s,
                  unsigned __int32 &ms) const
{
	DBC_REQUIRE(!is_not_a_date_time());

    h = hour();
    m = minute();
    s = sec();
    ms = milli();
}

///////////////////////////////////////////////////////////////////////////////
///
/// Set the stored date using the internal representation format.
/// @param date Date in the format @c YYYYMMDD, where
///     - <tt>YYYY</tt> = year (0004 to 4004)
///     - <tt>  MM</tt> = month (1 to 12)
///     - <tt>  DD</tt> = day (1 to 31)
/// 
/// @pre Components of @c date must be in their appropriate ranges.
/// @pre Resulting date/time must not be earlier than midnight, 1st March 0004
/// @pre Resulting date/time must be earlier than midnight, 1st March 4004
///
void
DateTime::setDate(unsigned long date)
{
	const unsigned long yr   = date / 10000;
	const unsigned long mth  = (date / 100) % 100;
	const unsigned long dy   = date % 100;
	setDate(yr, mth, dy);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine if @c yr, @c mth and @c day are valid in combination.
/// @param yr the year to be checked
/// @param mth the month to be checked
/// @param dy is the day of the month to be checked
/// @return @c true if the parameters specify a valid combination, @c false
/// otherwise.
/// @sa @c daysInMonth(), @c isValidMonth(), @c isValidYear()
///
bool
DateTime::isValidYMD(unsigned __int32 yr, unsigned __int32 mth, unsigned __int32 dy)
{
	return isValidYear(yr) && isValidMonth(mth, yr) &&
		(dy <= daysInMonth(mth, yr));
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine if @c h, @c m, @c s, and @c ms are valid in combination.
/// @param h is the hour to be checked.
/// @param m is the number of minutes to be checked.
/// @param s is the number of seconds to be checked.
/// @param ms is the number of seconds to be checked.
/// @return @c true if the parameters specify a valid combination, @c false
/// otherwise.
/// @sa @c isValidHour(), @c isValidMinute(), @c isValidSecond(),
/// isValidMilliSecond()
///
bool
DateTime::isValidHMSms(unsigned __int32 h, unsigned __int32 m, unsigned __int32 s, unsigned __int32 ms)
{
	return isValidHour(h) && isValidMinute(m) && isValidSecond(s) &&
		isValidMilliSecond(ms);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine if @c date may be stored in an instance of @c DateTime.
/// @param date date to be checked in @c YYYYMMDD format, where
///        <pre>YYYY</pre> = year
///        <pre>  MM</pre> = month
///        <pre>  DD</pre> = day
/// @return @c true if @c date is valid, @c false otherwise.
/// @sa @c daysInMonth(), @c isValidMonth(), @c isValidYear()
///
bool
DateTime::isValidDate(unsigned long date)
{
	const unsigned __int32 yr = date / 10000;
	const unsigned __int32 mth = (date / 100) % 100;
	const unsigned __int32 dy = date % 100;

	return isValidMonth(mth, yr) && isValidYear(yr) && (dy >= 1) &&
	      (dy <= daysInMonth(mth, yr));
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine if @c time may be stored in an instance of @c DateTime.
/// @param time time to be checked in @c HHMMSSmmm format, where
///        <pre> HH</pre> = hour
///        <pre> MM</pre> = minute
///        <pre> SS</pre> = second
///        <pre>mmm</pre> = millisecond
/// @return @c true if @c time is valid, @c false otherwise.
/// @sa @c isValidHour(), @c isValidMinute(), @c isValidSecond(),
///     @c isValidMilliSecond()
///
bool
DateTime::isValidTime(unsigned long time)
{
	const unsigned __int32 h = time / 10000000;
	const unsigned __int32 m = (time / 100000) % 100;
	const unsigned __int32 s = (time / 1000) % 100;
	const unsigned __int32 ms = time % 1000;

	return isValidHour(h) && isValidMinute(m) && isValidSecond(s) &&
	       isValidMilliSecond(ms);
}

///////////////////////////////////////////////////////////////////////////////

///
/// Determine if @c t is within the range storeable by @c DateTime.
/// @param t number of seconds since 1st January 1970
/// @return @c true if @c t is in range, @c false otherwise.
///
bool
DateTime::isValid(time_t t)
{
	// Modifications now allow anything that can be stored in either
	// a 32-bit time_t or 64-bit time_t to be stored in this
	// class.

	t; // unused
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Set the stored date using discrete year, month and day values.
/// @pre <tt>yr, mth</tt> and <tt>dy</tt> must be in their appropriate ranges.
/// @pre Resulting date/time must not be earlier than midnight, 1st March 0004
/// @pre Resulting date/time must be earlier than midnight, 1st March 4004
///
void
DateTime::setDate(unsigned __int32 yr,
                  unsigned __int32 mth,
                  unsigned __int32 dy)
{
    if(IsNull())asPtime() = DT_TM_EPOCH;

    asPtime() = ptime(gregorian::date(
					  numeric_cast<boost::gregorian::date::year_type>(yr),
		              numeric_cast<boost::gregorian::date::month_type>(mth),
		              numeric_cast<boost::gregorian::date::day_type>(dy)),
		              time_of_day());
}

///////////////////////////////////////////////////////////////////////////////
///
/// Set the stored time using the internal representation format.
/// @param time Time in the format @c HHMMSSmmm, where
///     - <tt>  HH</tt> = hour (00 to 23)
///     - <tt>  MM</tt> = minute (00 to 59)
///     - <tt>  SS</tt> = second (00 to 59)
///     - <tt> mmm</tt> = millisecond (000 to 999)
/// 
/// @pre Components of @c time must be in their appropriate ranges.
/// @pre Resulting date/time must not be earlier than midnight, 1st March 0004
/// @pre Resulting date/time must be earlier than midnight, 1st March 4004
///
void
DateTime::setTime(unsigned long time)
{
	const unsigned long hrs  = time / 10000000;
	const unsigned long mins = (time % 10000000) / 100000;
	const unsigned long secs = (time % 100000) / 1000;
	const unsigned long frac = time % 1000;

	setTime(hrs, mins, secs, frac);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Set the stored time using discrete hour, minute, second and millisecond
/// values.
/// @param h   Hour, in the range [0, 23].
/// @param m   Minute, in the range [0, 59].
/// @param s   Second, in the range [0, 59].
/// @param ms  Millisecond, in the range [0, 999].
/// @pre <tt>h, m, s</tt> and <tt>ms</tt> must be in their appropriate ranges.
/// @pre Resulting date/time must not be earlier than midnight, 1st March 0004
/// @pre Resulting date/time must be earlier than midnight, 1st March 4004
///
void
DateTime::setTime(unsigned __int32 h,
                  unsigned __int32 m,
                  unsigned __int32 s,
                  unsigned __int32 ms)
{
	if (h >   23) throw out_of_range("Hours");
	if (m >   59) throw out_of_range("Minutes");
	if (s >   59) throw out_of_range("Seconds");
	if (ms > 999) throw out_of_range("Milli-seconds");

    if(IsNull())asPtime() = DT_TM_EPOCH;

	asPtime() = ptime(date(),
		              hours(numeric_cast<hour_type>(h))
					+ minutes(numeric_cast<min_type>(m))
					+ seconds(numeric_cast<sec_type>(s))
					+ milliseconds(numeric_cast<fractional_seconds_type>(ms)));

	DBC_ENSURE(time_of_day().hours() == h);
	DBC_ENSURE(time_of_day().minutes() == m);
	DBC_ENSURE(time_of_day().seconds() == s);
	DBC_ENSURE((time_of_day().fractional_seconds()
        / (time_duration::ticks_per_second() / 1000)) == ms);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @return this @c DateTime as a local time.
/// @sa toLocalTime()
///
DateTime
DateTime::asLocalTime() const
{
	DateTime result(*this);
	result.toLocalTime();
	return result;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Convert from GMT to local time.
/// @pre Time/date to be converted does not come before midnight, 1st Jan 1970
///      (see @b Note below).
/// @note
///     - This function assumes the DateTime object currently stores the time in
///       GMT (UTC); if it doesn't, the result will be incorrect.
///     - This function is constrained by the C library implementation, so dates
///       before 1970 cannot be converted. The latest date that may be converted
///       depends on the number of bits in the @c time_t type; if it is 32-bit,
///       this will be some time in 2038. Therefore this function will fail if
///       the date/time to be converted can't be represented in a @c time_t.
///
void
DateTime::toLocalTime()
{
	DBC_OLD(
		DateTime old_this(*this);
		static int nesting_count = 0;
		NestingCount nest(nesting_count);
		);

    asPtime() = us_eastern::utc_to_local(*this);

	DBC_ENSURE(nest.isNested() || (asGMT() == old_this));
}

///////////////////////////////////////////////////////////////////////////////
// --> begin
///
/// @return this DateTime converted to GMT.
///
DateTime
DateTime::asGMT() const
{
	DBC_REQUIRE(!is_not_a_date_time());
	DBC_REQUIRE(date() >= gregorian::date(1970,01,01));
	DateTime result(*this);
	result.toGMT();
	return result;
}
// --> end

///////////////////////////////////////////////////////////////////////////////
///
/// Convert from local time to GMT.
/// @pre Time/date to be converted does not come before midnight, 1st Jan 1970
///      (see @b Note below).
/// @note
///     - This function assumes the DateTime object currently stores local time;
///       if it doesn't, the result will be incorrect.
///     - This function is constrained by the C library implementation, so dates
///       before 1970 cannot be converted. The latest date that may be converted
///       depends on the number of bits in the @c time_t type; if it is 32-bit,
///       this will be some time in 2038. Therefore this function will fail if
///       the date/time to be converted can't be represented in a @c time_t.
///
void
DateTime::toGMT()
{
	DBC_OLD(
		DateTime old_this(*this);
		static int nesting_count = 0;
		NestingCount nest(nesting_count);
		);

	DBC_REQUIRE(!is_not_a_date_time());

    asPtime() = us_eastern::local_to_utc(*this);

	DBC_ENSURE(nest.isNested() || asLocalTime() == old_this);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Add @c span to a @c DateTime instance.
/// @param span Time span to be added
/// @return Reference to @c this.
/// @pre addObeysRange(span)
///
DateTime &
DateTime::operator+=(const DateTimeSpan &span)
{
    asPtime() += span.toPosixDuration();

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Subtract @c span from a @c DateTime instance.
/// @param span Time span to be subtracted
/// @return Reference to @c this.
/// @note The new date and time must not exceed the minimum or maximum values
///       representable by @c DateTime.
///
DateTime &
DateTime::operator-=(const DateTimeSpan& span)
{
    asPtime() -= span.toPosixDuration();

    return *this;
}


DateTimeSpan 
DateTime::operator-(const DateTime & other) const
{
	return DateTimeSpan(other, *this);
}

///////////////////////////////////////////////////////////////////////////////

///
/// Assign a date/time value of type @c time_t to a @c DateTime instance.
/// @param t Value to be assigned.
/// @return Reference to @c this.
/// @note This function is not available on Win32 platforms. For Win32 platforms
///       there is a similar version of this function which takes a value of
///       type @c __time64_t, which can also take values of type @c time_t.
/// @sa @c assign(__time64_t)
///
DateTime &
DateTime::assign(time_t t)
{
	asPtime() = boost::posix_time::from_time_t(t);
    return *this;
}

unsigned __int32
DateTime::year() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return date().year();
}

unsigned __int32
DateTime::month() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return date().month();
}

unsigned __int32
DateTime::day() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return date().day();
}

unsigned __int32
DateTime::dayOfWeek() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	unsigned __int32 x = date().day_of_week();
	x = (x + 1) % 7;
	return x;
}

unsigned __int32
DateTime::hour() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return time_of_day().hours();
}

unsigned __int32
DateTime::minute() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return time_of_day().minutes();
}

unsigned __int32
DateTime::sec() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return time_of_day().seconds();
}

unsigned __int32
DateTime::milli() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return numeric_cast<unsigned __int32>(time_of_day().total_milliseconds() % int64_t(1000));
}

unsigned long
DateTime::getDate() const
{
	if (date().is_special() )
	{
		return 0;
	}
	else
	{
		const gregorian::date::ymd_type ymd = date().year_month_day();

		return ymd.year * 10000
			   + ymd.month * 100
			   + ymd.day;
	}
}

unsigned long
DateTime::getTime() const
{
	return numeric_cast<unsigned long>(
		  time_of_day().hours() * 10000000
		+ time_of_day().minutes() * 100000
		+ time_of_day().seconds() * 1000
		+ (time_of_day().total_milliseconds() % _MILLIS_PER_SEC));
}

bool
DateTime::isLeapYear() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return gregorian_calendar::is_leap_year(date().year());
}

unsigned __int32
DateTime::daysInMonth(unsigned __int32 mth, unsigned __int32 yr)
{
	return gregorian_calendar::end_of_month_day(
		numeric_cast<unsigned short>(yr),
		numeric_cast<unsigned short>(mth));
}

unsigned __int32
DateTime::daysInMonth(unsigned __int32 mth) const
{
	return gregorian_calendar::end_of_month_day(
		date().year(),
		numeric_cast<unsigned short>(mth));
}

unsigned __int32
DateTime::daysInMonth() const
{
	return gregorian_calendar::end_of_month_day(
		date().year(),
		date().month());
}

///////////////////////////////////////////////////////////////////////////////
///
/// Return the stored date/time as the number of seconds since midnight,
/// 1st January 1970.
/// @return Number of seconds since midnight, 1st January 1970.
unsigned __int64
DateTime::secsSince1970() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	const time_duration time_since = asPtime() - DT_TM_EPOCH;
	uint64_t offset = time_since.hours() * _MINS_PER_HOUR * _SECS_PER_MIN;
	offset += time_since.minutes() * _SECS_PER_MIN;
	offset += time_since.seconds();
	return offset;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Return the stored date/time as @c time_t.
/// @return Number stored date/time as value of type @c time_t.
time_t
DateTime::toTime_t() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return static_cast<time_t>(secsSince1970());
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the current date and time.
/// @return @c DateTime instance containing the current date and time, according
///         to GMT.
///
DateTime
DateTime::now()
{
	return DateTime(boost::posix_time::second_clock::universal_time());
}

///////////////////////////////////////////////////////////////////////////////
///
/// Add @c span to a @c DateTime instance, returning a new instance.
/// @param span Time span to be added
/// @return Another @c DateTime instance containing the result.
/// @note The new date and time must not exceed the minimum or maximum values
///       representable by @c DateTime.
///
DateTime
DateTime::operator+(const DateTimeSpan &span) const
{
    DateTime ret(*this);
    ret += span;
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Subtract @c span from a @c DateTime instance, returning a new instance.
/// @param span Time span to be subtracted
/// @return Another @c DateTime instance containing the new result.
/// @note The new date and time must not exceed the minimum or maximum values
///       representable by @c DateTime.
///
DateTime
DateTime::operator-(const DateTimeSpan &span) const
{
    DateTime ret(*this);
    ret -= span;
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine whether @c year can be stored in an instance of @c DateTime.
/// Valid year values range from 1400 to 10000 inclusive.
/// @return @c true if @c year is in the range [1400, 10000], @c false otherwise.
///
bool
DateTime::isValidYear(unsigned __int32 year)
{
	return (year >= 1400 && year <= 10000);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine whether @c month can be stored in an instance of @c DateTime.
/// Valid month values range from 1 to 12 inclusive.
/// @return @c true if the month in the specified year is valid,
/// @c false otherwise.
///
bool
DateTime::isValidMonth(unsigned __int32 mth, unsigned __int32 yr)
{
	return isValidYear(yr) &&
			(mth >= 1 && mth <= 12) &&
			(yr > 4 || mth >= 3) &&
			(yr < 4004 || mth <= 2);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine whether @c hour can be stored in an instance of @c DateTime.
/// Valid hour values range from 0 to 23 inclusive.
/// @return @c true if @c hour is in the range [0, 23], @c false otherwise.
///
bool
DateTime::isValidHour(unsigned __int32 hour)
{
	return (hour >= 0 && hour <= 23);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine whether @c minute can be stored in an instance of @c DateTime.
/// Valid minute values range from 0 to 59 inclusive.
/// @return @c true if @c minute is in the range [0, 59], @c false otherwise.
///
bool
DateTime::isValidMinute(unsigned __int32 minute)
{
	return (minute >= 0 && minute <= 59);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine whether @c sec can be stored in an instance of @c DateTime.
/// Valid second values range from 0 to 59 inclusive.
/// @return @c true if @c sec is in the range [0, 59], @c false otherwise.
///
bool
DateTime::isValidSecond(unsigned __int32 sec)
{
	return (sec >= 0 && sec <= 59);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine whether @c ms can be stored in an instance of @c DateTime.
/// Valid millisecond values range from 0 to 999 inclusive.
/// @return @c true if @c ms is in the range [0, 999], @c false otherwise.
///
bool
DateTime::isValidMilliSecond(unsigned __int32 ms)
{
	return (ms >= 0 && ms <= 999);
}

std::string
DateTime::getFormattedDate() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	return (boost::format( "%s %02d\\%s\\%04d" ) 
						% DateTime::DAY_ABBREV[dayOfWeek()]
						% day()
						% DateTime::MONTH_ABBREV[month()] 
						% year()
					).str();
}

///////////////////////////////////////////////////////////////////////////////
std::string
DateTime::ToYYYYMMDD_hhmm() const
{
    if(IsNull())return std::string();

    char formatted[14];

    sprintf_s(formatted, sizeof(formatted), "%08d-%04d", Dt(), Tm()/100);

    return formatted;
}
///////////////////////////////////////////////////////////////////////////////
std::string
DateTime::ToYYYYMMDDhhmm() const
{
    if(IsNull())return std::string();

    char formatted[14];

    sprintf_s(formatted, sizeof(formatted), "%08d.%04d", Dt(), Tm()/100);

    return formatted;
}
///////////////////////////////////////////////////////////////////////////////
DateTime 
DateTime::FromYYYYMMDD_hhmm(const std::string& YYYYMMDD_hhmm)
{
    std::string date_string, time_string;
    boost::date_time::split(YYYYMMDD_hhmm, '-', date_string, time_string);

    const long date = atoi(date_string.c_str());
    const long time = atoi(time_string.c_str()) * 100 * 1000;

    return isValidDate(date) && isValidTime(time) 
            ?   DateTime(date, time)
            :   DateTime(not_a_date_time);
}
///////////////////////////////////////////////////////////////////////////////
std::string
DateTime::ToYYYYMMDD_hhmmss() const
{
    if(IsNull())return std::string();

    char formatted[16];

    sprintf_s(formatted, sizeof(formatted), "%08d-%06d", Dt(), Tm());

    return formatted;
}
///////////////////////////////////////////////////////////////////////////////
std::string
DateTime::ToYYYYMMDDhhmmss() const
{
    if(IsNull())return std::string();

    char formatted[16];

    sprintf_s(formatted, sizeof(formatted), "%08d.%06d", Dt(), Tm());

    return formatted;
}
///////////////////////////////////////////////////////////////////////////////
DateTime 
DateTime::FromYYYYMMDD_hhmmss(const std::string& YYYYMMDD_hhmmss)
{
    std::string date_string, time_string;
    
    boost::date_time::split(YYYYMMDD_hhmmss, '-', date_string, time_string);

    const long date = atoi(date_string.c_str());
    const long time = atoi(time_string.c_str()) * 1000;

    return isValidDate(date) && isValidTime(time) 
            ?   DateTime(date, time)
            :   DateTime(not_a_date_time);
}
///////////////////////////////////////////////////////////////////////////////
DateTime 
DateTime::FromDBString(const std::string& YYYYMMDD_hhmmss000)
{
    DateTime result(Null);

    try
    {
        SYSTEMTIME st;
        float seconds;
        memset(&st, 0, sizeof(SYSTEMTIME));
        sscanf_s(YYYYMMDD_hhmmss000.c_str(), "%d-%d-%d %d:%d:%f",
            &st.wYear,
            &st.wMonth,
            &st.wDay,
            &st.wHour,
            &st.wMinute,
            &seconds);

            st.wSecond          = seconds;
            st.wMilliseconds    = 1000* (seconds - st.wSecond); 

        if(isValidYMD(st.wYear, st.wMonth, st.wDay)
        && isValidHMSms(st.wHour, st.wMinute, seconds, st.wMilliseconds) )
        {
            result.setDate(st.wYear, st.wMonth, st.wDay);
            result.setTime(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        }
    }
    catch(...)
    {
    }

    return result;
}
///////////////////////////////////////////////////////////////////////////////
std::string
DateTime::ToDBString() const
{
    if(IsNull())return std::string();

    char formatted[25];

    sprintf_s(formatted, sizeof(formatted), 
        "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            year(), month(), day(), hour(), minute(), sec(), milli());

    return formatted;
}
///////////////////////////////////////////////////////////////////////////////
///
/// Return the stored date/time as the number of milliseconds since the epoch,
/// 1st March 0004.
/// @return Number of milliseconds since the epoch.
///
unsigned __int64
DateTime::toMillis() const
{
	DBC_REQUIRE(!is_not_a_date_time());

	const long days_count = (asPtime().date() - DT_TM_EPOCH.date()).days();

    return MILLI_SEC_PER_DAY * days_count + (asPtime().time_of_day() - DT_TM_EPOCH.time_of_day()).total_milliseconds();
}
///////////////////////////////////////////////////////////////////////////////
unsigned __int32 
DateTime::season( ) const
{
	DBC_REQUIRE(!is_not_a_date_time());

    unsigned __int32 season =0;

    switch( date().month() )
    {
        case 1:
        case 2:
            season = 1; // Winter
        break;

        case 3:
            season = date().day() < 22 ? 1 : 2;
        break;

        case 4:
        case 5:
            season = 2; // Spring
        break;

        case 6:
            season = date().day() < 22 ? 2 : 3;
        break;

        case 7:
        case 8:
            season = 3; // Summer
        break;

        case 9:
            season = date().day() < 22 ? 3 : 4;
        break;

        case 10:
        case 11:
            season = 4; // Autumn
        break;

        case 12:
            season = date().day() < 22 ? 4 : 1;
        break;
    }

    return season;
}
///////////////////////////////////////////////////////////////////////////////
unsigned __int32 
DateTime::quarter( ) const
{
	DBC_REQUIRE(!is_not_a_date_time());

    return 1 + (date().month()-1) / 3;
}
///////////////////////////////////////////////////////////////////////////////
double 
DateTime::toDouble() const
{
    double raw_dt_tm = NaN;

    if(IsValid())
    {
        SYSTEMTIME st;
        st.wYear            = year();
        st.wMonth           = month();
        st.wDay             = day();
        st.wHour            = hour();
        st.wMinute          = minute();
        st.wSecond          = sec();
        st.wMilliseconds    = milli();

        SystemTimeToVariantTime(&st, &raw_dt_tm);
    }

	return raw_dt_tm;
}
///////////////////////////////////////////////////////////////////////////////
static const double ONETHOUSANDMILLISECONDS = 0.0000115740740740;
///////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeToVariantTime000(/*input*/ SYSTEMTIME st, /*output*/double *dVariantTime)
{
    BOOL retVal = true;

    WORD wMilliSeconds = st.wMilliseconds; // save the milli second  information

    st.wMilliseconds = 0; // pass 0 milliseconds to the function and get  
                          // the converted value without milliseconds
    double dWithoutms;
    retVal = SystemTimeToVariantTime(&st, &dWithoutms) ;

    // manually convert the millisecond information into variant 
    // fraction and add it to system converted value
    double OneMilliSecond = ONETHOUSANDMILLISECONDS/1000 ;
    *dVariantTime = dWithoutms + (OneMilliSecond * wMilliSeconds);

    return retVal;
}
///////////////////////////////////////////////////////////////////////////////
BOOL VariantTimeToSystemTime000(/*input*/ double dVariantTime, /*output*/SYSTEMTIME *st)
{
    BOOL retVal = TRUE;

    double halfsecond = ONETHOUSANDMILLISECONDS / 2.0; 
    // ONETHOUSANDMILLISECONDS is equal to 0.0000115740740740

    retVal = VariantTimeToSystemTime(dVariantTime - halfsecond, st); 

    // this takes care of rounding problem with 
    // VariantTimetoSystemTime function
    if (retVal == FALSE)
    {
        return retVal;
    }

    double fraction = dVariantTime - (int) dVariantTime; 
    // extracts the fraction part

    double hours; 
    hours = fraction = (fraction - (int)fraction) * 24;

    double minutes;
    minutes = (hours - (int)hours) * 60;

    double seconds;
    seconds = (minutes - (int)minutes) * 60;

    double milliseconds;
    milliseconds = (seconds - (int)seconds) * 1000;

    milliseconds = milliseconds + 0.5; // rounding off millisecond to the 
                                       // nearest millisecond 
    if (milliseconds < 1.0 || milliseconds > 999.0) //Fractional 
                          // calculations may yield in results like
        milliseconds = 0; // 0.00001 or 999.9999 which should actually 
                          // be zero (slightly above or below limits 
                          // are actually zero)

    if (milliseconds) 
        st->wMilliseconds = (WORD) milliseconds;
    else  // if there is 0 milliseconds, then we don't have the problem !!
        retVal = VariantTimeToSystemTime(dVariantTime, st); // 

    return retVal;
}
///////////////////////////////////////////////////////////////////////////////

} // namespace mr

///////////////////////////////////////////////////////////////////////////////
///
/// Output @c date_time to @c stream in textual form.
/// @param stream    destination data stream
/// @param date_time @c DateTime instance to be output
/// @return Reference to @c stream.
///
std::ostream&
operator<<(std::ostream& stream,
           const mr::DateTime& date_time)
{
	using namespace boost;
	using namespace std;
	if (date_time.is_not_a_date_time())
	{
		stream << "NOT A DATE TIME";
	}
	else
	{
		stream << mr::DateTime::DAY_ABBREV[date_time.dayOfWeek()] << ' '
			<< setfill('0') << setw(2) << static_cast<boost::uint32_t>(date_time.day()) << ' '
			<< mr::DateTime::MONTH_ABBREV[date_time.month()] << ' '
			<< setfill('0') << setw(4) << static_cast<uint32_t>(date_time.year()) << "  "
			<< setfill('0') << setw(2) << static_cast<uint32_t>(date_time.hour()) << ':'
			<< setfill('0') << setw(2) << static_cast<uint32_t>(date_time.minute()) << ':'
			<< setfill('0') << setw(2) << static_cast<uint32_t>(date_time.sec()) << '.'
			<< setfill('0') << setw(3) << static_cast<uint32_t>(date_time.milli());
	}

    return stream;
}
///////////////////////////////////////////////////////////////////////////////
