#ifndef __MRDATEFUNCS_H__
#define __MRDATEFUNCS_H__

// This is included in front of the boost/date_time library version because
// the validation range is too restrictive for our purposes.
// This is a modified greg_year only in so much that the range of values
// has been widened.
#include "DateTime/DateTimeAPI.h"
#include "DateTime/MRDateTime.h"

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

	///
	/// Date/time and lunar calendar utility functions.
    ///
    namespace DateFuncs
    {
		inline long daysBetween(const DateTime &from, const DateTime &to);
        inline long daysBetween(const DateTime &to);

		long MR_DATE_TIME_API calculateWorkingDaysBetween(const mr::DateTime &from, const mr::DateTime &to);

		inline long workingDaysBetween(const DateTime &from, const DateTime &to) { return calculateWorkingDaysBetween(from, to); }
		long MR_DATE_TIME_API workingDaysBetween(boost::int32_t from, boost::int32_t to);
        inline long workingDaysBetween(const DateTime &to);

        DateTime MR_DATE_TIME_API daysFrom(long days, const DateTime &from = DateTime::Epoch);
		boost::int32_t MR_DATE_TIME_API daysFrom(long days, boost::int32_t from);
        DateTime MR_DATE_TIME_API workingDaysFrom(long days, const DateTime &from = DateTime::Epoch);
		boost::int32_t MR_DATE_TIME_API workingDaysFrom(long days, boost::int32_t from);


        inline double moonPhase(const DateTime&, long&) {return 0;}
        inline double moonPhase(const DateTime&)        {return 0;}
#pragma deprecated(moonPhase)

    };
};

///////////////////////////////////////////////////////////////////////////////

/*****  Inline Functions  *****/

///
/// Calculate the number of days between the Epoch (01/01/1970) and @c to.
/// @param to The bounding date.
/// @return The number of days between 01/01/1970 and @c to.
///
inline long
mr::DateFuncs::daysBetween(const DateTime &to)
{
    return daysBetween(DateTime::Epoch, to);
}

inline long
mr::DateFuncs::daysBetween(const DateTime &from, const DateTime &to)
{
	return (to.asPtime() - from.asPtime()).hours() / 24;
}
///////////////////////////////////////////////////////////////////////////////
///
/// Calculate the number of weekdays between the Epoch (01/01/1970) and @c to.
/// @param to The bounding date.
/// @return The number of weekdays between 01/01/1970 and @c to.
///
inline long
mr::DateFuncs::workingDaysBetween(const DateTime &to)
{
    return workingDaysBetween(DateTime::Epoch, to);
}
///////////////////////////////////////////////////////////////////////////////
#endif /* __MRDATEFUNCS_H__ */
///////////////////////////////////////////////////////////////////////////////
