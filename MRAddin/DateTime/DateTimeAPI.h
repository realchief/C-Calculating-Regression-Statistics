#pragma once

#ifdef MR_DATE_TIME_EXPORTS
#define MR_DATE_TIME_API __declspec(dllexport)
#else
#define MR_DATE_TIME_API __declspec(dllimport)
#endif

#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable : 4311 4312 4511 4512)
#endif

#include <boost/date_time/date.hpp>
#include <boost/date_time/gregorian_calendar.hpp>
#include <boost/date_time/gregorian/greg_calendar.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>


#ifdef _MSC_VER
#	pragma push_macro("new")
#	undef new
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

#ifdef _MSC_VER
#pragma pop_macro("new")
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace mr
{

	class DateTime;
	class DateTimeSpan;

} // namespace mr
