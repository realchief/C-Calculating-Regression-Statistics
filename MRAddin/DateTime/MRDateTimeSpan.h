#ifndef __MRDATETIMESPAN_H__
#define __MRDATETIMESPAN_H__

// This is included in front of the boost/date_time library version because
// the validation range is too restrictive for our purposes.
// This is a modified greg_year only in so much that the range of values
// has been widened.
#include "DateTime/DateTimeAPI.h"
#include "DesignByContract.h"

#ifndef  MR_DATE_TIME_EXPORTS
#	ifdef PW_COMPILER_AUTOLINK
#		pragma comment(lib, "DateTime.lib") // for PW Compiler
#	endif // PW_COMPILER_AUTOLINK
#endif

///////////////////////////////////////////////////////////////////////////////

/*****  Forward Declarations  *****/

namespace mr
{
    class DateTime;
};

///////////////////////////////////////////////////////////////////////////////

/*****  Data Structures, Types and Classes  *****/

namespace mr
{
	using boost::numeric_cast;
	using boost::int64_t;
	using boost::uint64_t;

	///
	/// Represents an arbitrary period of time.
	/// <p>
	/// @c DateTimeSpan is used to perform arithmetic with the @c DateTime
	/// class. Two @c DateTime instances may be used to create a @c DateTimeSpan
	/// instance, which will then contain the difference in time between those
	/// two @c DateTime instances. Alternatively, a @c DateTimeSpan instance can
	/// be created by specifying a specific time period. A @c DateTimeSpan
	/// instance can be added to or subtracted from a @c DateTime instance,
	/// modifying its date and time appropriately. @c DateTimeSpan instances may
	/// also be added to and subtracted from each other.
	/// </p><p>
	/// A @c DateTimeSpan may represent both positive and negative time periods.
	/// </p><p>
	/// <b>Example 1 - Get the date two weeks from now</b>
	/// @code
	/// #include <iostream>
	/// #include <iomanip>
	///
	/// #include "DateTime/MRDateTime.h"
	/// #include "DateTime/MRDateTimeSpan.h"
	///
	/// int
	/// main(int argc,
	///      char *argv[])
	/// {
	///     mr::DateTimeSpan span(28, 0, 0, 0, 0);  // Set up time span
	///     mr::DateTime dt(DateTime::now());       // Get current date and time
	/// 
	///     dt += span;
	/// 
	///     std::cout << "Date two weeks from now: " << dt.day() << ' '
	///         << DateTime::MONTH[dt.month()] << ' '
	///         << std::setfill('0') << std::setw(4) << dt.year() << std::endl;
	/// 
	///     return 0;
	/// }
	/// @endcode
	/// </p><p>
	/// <b>Example 2 - Determine the time taken by a long operation</b>
	/// @code
	/// #include <iostream>
	///
	/// #include "DateTime/MRDateTime.h"
	/// #include "DateTime/MRDateTimeSpan.h"
	///
	/// int
	/// main(int argc,
	///      char *argv[])
	/// {
	///     mr::DateTime before(DateTime::now());    // Get start time
	/// 
	///     // ... perform lengthy operation here ...
	/// 
	///     mr::DateTime after(DateTime::now());     // Get finish time
	///     mr::DateTimeSpan span(before, after);    // Get time taken
	/// 
	///     std::cout << "Time taken: " << span.days() << " day, "
	///         << span.hours() << " hr, " << span.mins() << " min, "
	///         << span.secs() << " sec, " << span.millis() << " millisec"
	///         << std::endl;
	/// 
	///     return 0;
	/// }
	/// @endcode
	/// </p>
	/// @sa @c DateTime class
	///
    class MR_DATE_TIME_API DateTimeSpan
		:	private boost::less_than_comparable<DateTimeSpan>,
			private boost::equality_comparable<DateTimeSpan>
    {
    public:
		///
		/// Default constructor.
		/// Initialises the time period to zero.
		///
        DateTimeSpan();

		///
		/// Copy constructor.
		/// @param other @c DateTimeSpan instance to be copied.
		///
        DateTimeSpan(const DateTimeSpan& other);

        ///
        /// Create a @c DateTimeSpan, specifying a time period in milliseconds.
        /// @param ms Number of milliseconds.
        ///
		explicit DateTimeSpan(int64_t ms);

        DateTimeSpan(const DateTime& from, const DateTime& to);

        DateTimeSpan(long days, long hrs, long mins,
                     long secs, long millis);

		explicit DateTimeSpan(const boost::posix_time::time_duration & date_time_span);

        ~DateTimeSpan();

        long days() const;
        long hours() const;
        long totalHours() const;
        long mins() const;
        long totalMins() const;
        long secs() const;
        int64_t totalSecs() const;
        long millis() const;
        int64_t totalMillis() const;

////    "DateTimeSpan::TtlDays()" returns the span as a total number of days
////    group:datetimespanfuncs
        double TtlDays  ( ) const   {return static_cast<double>(_ms)/86400000.0;}

////    "DateTimeSpan::TtlHours()" returns the span as a total number of hours
////    group:datetimespanfuncs        
        double TtlHours ( ) const   {return static_cast<double>(_ms)/ 3600000.0;}

////    "DateTimeSpan::TtlMins()" returns the span as a total number of minutes
////    group:datetimespanfuncs
        double TtlMins  ( ) const   {return static_cast<double>(_ms)/   60000.0;}

////    "DateTimeSpan::TtlSecs()" returns the span as a total number of seconds
////    group:datetimespanfuncs
        double TtlSecs  ( ) const   {return static_cast<double>(_ms)/    1000.0;}

		boost::posix_time::time_duration toPosixDuration() const;

        void set(const DateTime &from, const DateTime &to);
        void set(long days, long hrs = 0, long mins = 0, long secs = 0,
                 long millis = 0);

        DateTimeSpan &operator=(const DateTimeSpan &other);

        DateTimeSpan &operator+=(const DateTimeSpan &other);
        DateTimeSpan &operator-=(const DateTimeSpan &other);

        DateTimeSpan operator+(const DateTimeSpan &other) const;
        DateTimeSpan operator-(const DateTimeSpan &other) const;

        bool operator==(const DateTimeSpan &other) const;
        bool operator<(const DateTimeSpan &other) const;

    private:
        /// Time difference, in milliseconds.
        boost::int64_t _ms;

		static const int64_t _HOURS_PER_DAY = 24;
		static const int64_t _MINS_PER_HOUR = 60;
		static const int64_t _SECS_PER_MIN = 60;
		static const int64_t _MILLIS_PER_SEC = 1000;
    };

///////////////////////////////////////////////////////////////////////////////

/*****  Inline Functions  *****/

} // namespace mr

///////////////////////////////////////////////////////////////////////////////

#endif /* __MRDATETIMESPAN_H__ */
