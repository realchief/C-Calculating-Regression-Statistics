#include "Common.h"
#include "DateTime/DateTimeAPI.h"

#include "DateTime/MRDateTimeSpan.h"
#include "DateTime/MRDateTime.h"
#include "DebugNew.h"

namespace mr
{
	using namespace boost;
	using namespace boost::posix_time;
    using namespace boost::gregorian;
	using namespace std;

namespace
{
    template<typename Target, typename Source>
        Target narrow(Source x)
    {
#ifdef NDEBUG
        return static_cast<Target>(x);
#else
        return boost::numeric_cast<Target>(x);
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

DateTimeSpan::DateTimeSpan()
	:   _ms(0)
{
}

DateTimeSpan::DateTimeSpan(
	const DateTimeSpan& other)
	:	_ms(other._ms)
{
}

DateTimeSpan::DateTimeSpan(int64_t ms)
:	_ms(ms)
{
}

///
/// Create a @c DateTimeSpan containing the difference between two times.
/// @param from First date/time
/// @param to   Second date/time
///
DateTimeSpan::DateTimeSpan(const DateTime &from,
                           const DateTime &to)
{
    set(from, to);
}

///////////////////////////////////////////////////////////////////////////////

///
/// Create a @c DateTimeSpan from discrete time values.
/// @c days, @c hours, @c mins, @c secs and @c millis are not constrained to any
/// specific range, and may be negative.
/// @param days   Number of days
/// @param hours  Number of hours
/// @param mins   Number of minutes
/// @param secs   Number of seconds
/// @param millis Number of milliseconds
///
DateTimeSpan::DateTimeSpan(long days,
                           long hours,
                           long mins,
                           long secs,
                           long millis)
{
    set(days, hours, mins, secs, millis);
}

DateTimeSpan::DateTimeSpan(const boost::posix_time::time_duration & date_time_span) 
	: _ms(date_time_span.total_milliseconds())
{
}

///////////////////////////////////////////////////////////////////////////////

///
/// Destructor.
///
DateTimeSpan::~DateTimeSpan()
{
}

///////////////////////////////////////////////////////////////////////////////
///
/// Set a @c DateTimeSpan to the difference between two times.
/// @param from First date/time
/// @param to   Second date/time
///
void
DateTimeSpan::set(const DateTime &from,
                  const DateTime &to)
{
	DBC_REQUIRE(from.IsValid());
	DBC_REQUIRE(to.IsValid());

    _ms = (to.asPtime() - from.asPtime()).total_milliseconds();
}

///////////////////////////////////////////////////////////////////////////////
///
/// Set a @c DateTimeSpan from discrete time values.
/// @c days, @c hours, @c mins, @c secs and @c millis are not constrained to any
/// specific range, and may be negative.
/// @param days   Number of days
/// @param hours  Number of hours
/// @param mins   Number of minutes
/// @param secs   Number of seconds
/// @param millis Number of milliseconds
///
void
DateTimeSpan::set(long days,
                  long hours,
                  long mins,
                  long secs,
                  long millis)
{
    _ms = (days * 24LL * 60LL * 60LL * 1000LL)
        + (hours       * 60LL * 60LL * 1000LL)
        + (mins        * 60LL * 1000LL)
        + (secs               * 1000LL)
        + millis;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the number of days spanned by a @c DateTimeSpan.
/// @return Number of days spanned.
///
long
DateTimeSpan::days() const
{
    return narrow<long>(_ms / (24LL * 60LL * 60LL * 1000LL));
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the number of hours spanned by a @c DateTimeSpan.
/// @return Number of hours spanned after subtracting days.
/// @sa @c totalHours()
///
long
DateTimeSpan::hours() const
{
	return totalHours() % _HOURS_PER_DAY;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the @e total number of hours spanned by a @c DateTimeSpan.
/// @return Total number of hours spanned.
/// @sa @c hours()
///
long
DateTimeSpan::totalHours() const
{
    return narrow<long>(_ms / (1000LL * 60LL * 60LL));
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the number of minutes spanned by a @c DateTimeSpan.
/// @return Number of minutes spanned after subtracting days and hours.
/// @sa @c totalMins()
///
long
DateTimeSpan::mins() const
{
	return totalMins() % _MINS_PER_HOUR;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the @e total number of minutes spanned by a @c DateTimeSpan.
/// @return Total number of minutes spanned.
/// @sa @c mins()
///
long
DateTimeSpan::totalMins() const
{
    return narrow<long>(_ms / (1000LL * 60LL));
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the number of seconds spanned by a @c DateTimeSpan.
/// @return Number of seconds spanned after subtracting days, hours and minutes.
/// @sa @c totalSecs()
///
long
DateTimeSpan::secs() const
{
	return narrow<long>(totalSecs() % _SECS_PER_MIN);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the @e total number of minutes spanned by a @c DateTimeSpan.
/// @return Total number of seconds spanned.
/// @sa @c secs()
///
int64_t
DateTimeSpan::totalSecs() const
{
	return _ms / 1000LL;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the number of milliseconds spanned by a @c DateTimeSpan.
/// @return Number of milliseconds spanned after subtracting days, hours,
///         minutes and seconds.
/// @sa @c totalMillis()
///
long
DateTimeSpan::millis() const
{
    return narrow<long>(_ms % _MILLIS_PER_SEC);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Determine the @e total number of milliseconds spanned by a @c DateTimeSpan.
/// @return Total number of milliseconds spanned.
/// @sa @c millis()
///
int64_t
DateTimeSpan::totalMillis() const
{
	return _ms;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Assignment operator.
/// @param other @c DateTimeSpan to be assigned
/// @return Reference to @c this.
///
mr::DateTimeSpan &
DateTimeSpan::operator=(const DateTimeSpan &other)
{
    _ms = other._ms;
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Add one @c DateTimeSpan instance to another.
/// @param other @c DateTimeSpan object on right-hand side.
/// @return Reference to @c this.
///
mr::DateTimeSpan &
DateTimeSpan::operator+=(const DateTimeSpan &other)
{
    _ms += other._ms;
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Subtract one @c DateTimeSpan instance from another.
/// @param other @c DateTimeSpan object on right-hand side.
/// @return Reference to @c this.
///
mr::DateTimeSpan &
DateTimeSpan::operator-=(const DateTimeSpan &other)
{
    _ms -= other._ms;
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Add two @c DateTimeSpan instances, returning a new instance.
/// @param other @c DateTimeSpan object on right-hand side.
/// @return Another @c DateTimeSpan instance containing the result.
///
mr::DateTimeSpan
DateTimeSpan::operator+(const DateTimeSpan &other) const
{
    DateTimeSpan ret(*this);
    ret._ms += other._ms;
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Subtract two @c DateTimeSpan instances, returning a new instance.
/// @param other @c DateTimeSpan object on right-hand side.
/// @return Another @c DateTimeSpan instance containing the result.
///
mr::DateTimeSpan
DateTimeSpan::operator-(const DateTimeSpan &other) const
{
    DateTimeSpan ret(*this);
    ret._ms -= other._ms;
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
///
/// Equality operator.
/// @param other @c DateTimeSpan instance on right-hand side.
/// @return @c true if @c *this equals @c other, @c false if not equal.
///
bool
DateTimeSpan::operator==(const DateTimeSpan &other) const
{
    return (_ms == other._ms);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Less-than operator.
/// @param other @c DateTimeSpan instance on right-hand side.
/// @return @c true if @c *this is less than @c other, @c false otherwise.
///
bool
DateTimeSpan::operator<(const DateTimeSpan &other) const
{
    return (_ms < other._ms);
}

time_duration
DateTimeSpan::toPosixDuration() const
{
    return time_duration(totalHours(), mins(), secs(),
        millis() * (time_duration::ticks_per_second() / 1000));
}

///////////////////////////////////////////////////////////////////////////////

} // namespace mr