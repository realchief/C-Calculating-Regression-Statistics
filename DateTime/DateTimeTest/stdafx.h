#ifndef MR_DATE_TIME_TEST_STDAFX_H
#define MR_DATE_TIME_TEST_STDAFX_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

#include "windows.h"
#include "Platform.h"

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

//#include <afx.h> // to pacify MRUtil
//#include <afxole.h>

#include <exception>
#include <iomanip>
#include <iostream>
#include <vector>
#include <unordered_map>

#include <boost/cast.hpp>
#include <boost/crc.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/greg_calendar.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time_adjustor.hpp>

//#include "mrutil.h"
#include "WhichDay.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

