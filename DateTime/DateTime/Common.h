#ifndef MR_DATE_TIME_COMMON_H_INCLUDED
#define MR_DATE_TIME_COMMON_H_INCLUDED

#pragma once

#include "Platform.h"

#if _MSC_VER >= 1400
#	pragma warning(disable : 4996)
#endif	// #if _MSC_VER >= 1400

#pragma warning(push)
#pragma warning(disable : 4512 4511 4127 4244)

#include <iomanip>
#include <sys/timeb.h>

#include <boost/cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/current_function.hpp>
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

#ifdef _MSC_VER
#pragma pop_macro("new")
#endif

#include <ATLComTime.h>

#pragma warning(pop)

#endif
