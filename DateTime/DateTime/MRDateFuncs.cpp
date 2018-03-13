#include "Common.h"

#include "DateTime/DateTimeAPI.h"

#include "WhichDay.h"
// which_day has been replaced by WhichDay!!!
// converts 19991231 into serial date

#include "DateTime/MRDateFuncs.h"
#include "DateTime/MRDateTime.h"
#include "DateTime/MRDateTimeSpan.h"

#include "DesignByContract.h"
#include "DebugNew.h"

using namespace boost;
using namespace boost::date_time;
using namespace boost::posix_time;
using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*****  Globals  *****/
namespace
{
	const gregorian::date_duration ONE_DAY(1);
	const gregorian::date_duration TWO_DAYS(2);
}


///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

///
/// Calculate the number of days between @c from and @c to.
/// @param from The first date in the desired range.
/// @param to   The second date in the desired range.
/// @return Number of days between @c from and @c to. If @c from comes after
///         @c to, the result will be negative.
/// @note Only the dates, not the times, are taken into consideration.
///

bool is_weekday(const gregorian::date& dt)
{
	const int day_of_week = dt.day_of_week();
	return day_of_week == 0 || day_of_week == 6;
}

///
/// Calculate the number of weekdays between @c from and @c to.
/// @param from The first date in the desired range.
/// @param to   The second date in the desired range.
/// @return Number of weekdays between @c from and @c to. If @c from comes after
///         @c to, the result will be negative.
/// @note Only the dates, not the times, are taken into consideration.
/// @note This function does not take account of public holidays.
///

long
mr::DateFuncs::calculateWorkingDaysBetween(const mr::DateTime &from,
										   const mr::DateTime &to)
{
	/*static const int the_matrix[7][7] = { 
											{0,1,2,3,4,5,5}, 
											{4,0,1,2,3,4,4}, 
											{3,4,0,1,2,3,3}, 
											{2,3,4,0,1,2,2}, 
											{1,2,3,4,0,1,1}, 
											{0,1,2,3,4,0,0}, 
											{0,1,2,3,4,5,0}
										};*/

	static const mr::DateTime EPOCH_PTIME;//thu=4

	const long days = (to.asPtime() - from.asPtime()).hours() / 24;
	const long in_week_days = days % 7;

	if (in_week_days > 0)
	{
		const int dow_beg = (((from.asPtime() - EPOCH_PTIME).hours() / 24) + 4/*thu*/) % 7;
		const int dow_end = (dow_beg + in_week_days) % 7;
		const int diff = dow_end - dow_beg;
		return ((days / 7) * 5) + (diff < 0 ? 5 + diff + (dow_beg / 6) : diff - dow_end / 6 - dow_beg / 6);
	}
	else
	{
		const int dow_beg = (((to.asPtime() - EPOCH_PTIME).hours() / 24) + 4/*thu*/) % 7;
		const int dow_end = (dow_beg - in_week_days) % 7;
		const int diff = dow_end - dow_beg;
		return -(((-days / 7) * 5) + (diff < 0 ? 5 + diff + (dow_beg / 6) : diff - dow_end / 6 - dow_beg / 6)) - 2 * ( (dow_end + dow_beg) / 12);
	}
}

///////////////////////////////////////////////////////////////////////////////

long
mr::DateFuncs::workingDaysBetween(int32_t from, int32_t to)
{
	return WhichDay(to, true) - WhichDay(from, true);
}

///////////////////////////////////////////////////////////////////////////////
///
/// Calculate the date and time @c days from the specified date, @c from.
/// @param days Number of days
/// @param from Starting date
/// @return New @c DateTime instance containing the result.
///
mr::DateTime
mr::DateFuncs::daysFrom(long days,
                        const DateTime &from)
{
	return DateTime(from.asPtime() + gregorian::date_duration(days));
}

///////////////////////////////////////////////////////////////////////////////

int32_t
mr::DateFuncs::workingDaysFrom(long days, int32_t from)
{
	return UnWhichDay(WhichDay(from, true) + days, true);
}

///
/// Calculate the date and time @c days weekdays from the specified date,
/// @c from.
/// @param days Number of weekdays
/// @param from Starting date
/// @return New @c DateTime instance containing the result.
/// @note This function does not take account of public holidays.
///
mr::DateTime
mr::DateFuncs::workingDaysFrom(long days,
                               const DateTime &from)
{
	return DateTime(workingDaysFrom(days, from.getDate()), 0);
}

///////////////////////////////////////////////////////////////////////////////
