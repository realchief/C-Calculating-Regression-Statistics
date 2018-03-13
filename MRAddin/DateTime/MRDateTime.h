#ifndef __MRDATETIME_H__
#define __MRDATETIME_H__

#include "DateTime/DateTimeAPI.h"

namespace mr
{
    class DateTimeSpan;
    /// 
    /// Stores a specific date and time down to the millisecond level.
    /// <p>
    /// @c DateTime is capable of storing dates/times between midnight 1st Mar
    /// 0004 up to (but not including) midnight 1st Mar 4004. The date and time
    /// are stored internally as two unsigned integers: the first contains the
    /// date stored in the format @c YYYYMMDD, where
    ///     - <tt>YYYY</tt> = year (0004 to 4004)
    ///     - <tt>  MM</tt> = month (1 to 12)
    ///     - <tt>  DD</tt> = day (1 to 31)
    /// 
    /// The second contains the time stored in the format @c HHMMSSmmm, where
    ///     - <tt> HH</tt> = hour (00 to 23)
    ///     - <tt> MM</tt> = minute (00 to 59)
    ///     - <tt> SS</tt> = second (00 to 59)
    ///     - <tt>mmm</tt> = millisecond (000 to 999)
    /// 
    /// </p><p>
    /// Use @c DateTime by either constructing an instance with a specific date
    /// and/or time (various forms are accepted), or using the static @c now()
    /// function to get a @c DateTime instance containing the current date and
    /// time according to GMT. This can be converted to local time using the
    /// @c toLocalTime() function.
    /// </p><p>
    /// @c DateTime, and its companion class @c DateTimeSpan, provide a
    /// single platform-independent replacement for the MFC classes @c CTime,
    /// @c CTimeSpan, @c COleDateTime and @c COleDateTimeSpan. Use of the MFC
    /// classes is deprecated in favour of the @c MRDateTime class library.
    /// </p><p>
    /// Use of the old PW functions @c WhichDay(), @c UnWhichDay(), and other
    /// functions associated with them is also deprecated; instead, use
    /// @c DateTime in conjunction with the @c DateTimeSpan class and the
    /// @c DateFuncs functions such as @c mr::DateFuncs::daysBetween().
    /// </p><p>
    /// When using the @c DateTime class, the MRDateTime library must be linked
    /// with the program using it.
    /// </p><p>
    /// @b Example
    /// @code
    /// #include <iostream>
    /// #include <iomanip>
    /// 
    /// #include "DateTime/MRDateTime.h"
    /// 
    /// using mr::DateTime;
    /// 
    /// int
    /// main(int argc,
    ///      char *argv)
    /// {
    ///     using mr::DateTime;
    ///     using std::cout;
    ///     using std::endl;
    ///     using std::setw;
    ///     using std::setfill;
    /// 
    ///     DateTime gmt_time(DateTime::now());
    ///     DateTime loc_time(gmt_time);
    /// 
    ///     local_time.toLocalTime();
    /// 
    ///     // This replicates the standard DateTime streaming operator
    ///     cout << "Current time according to GMT: " << endl;
    ///     cout << "    " << DateTime::DAY_ABBREV[gmt_time.dayOfWeek()] << ' '
    ///         << setfill('0') << setw(2) << gmt_time.day() << ' '
    ///         << DateTime::MONTH_ABBREV[gmt_time.month()] << ' '
    ///         << setfill('0') << setw(4) << gmt_time.year() << "  "
    ///         << setfill('0') << setw(2) << gmt_time.hour() << ':'
    ///         << setfill('0') << setw(2) << gmt_time.minute() << ':'
    ///         << setfill('0') << setw(2) << gmt_time.sec() << '.'
    ///         << setfill('0') << setw(3) << gmt_time.milli() << endl;
    /// 
    ///     cout << endl << "Current local time: " << endl;
    ///     cout << "    " << DateTime::DAY_ABBREV[loc_time.dayOfWeek()] << ' '
    ///         << setfill('0') << setw(2) << loc_time.day() << ' '
    ///         << DateTime::MONTH_ABBREV[loc_time.month()] << ' '
    ///         << setfill('0') << setw(4) << loc_time.year() << "  "
    ///         << setfill('0') << setw(2) << loc_time.hour() << ':'
    ///         << setfill('0') << setw(2) << loc_time.minute() << ':'
    ///         << setfill('0') << setw(2) << loc_time.sec() << '.'
    ///         << setfill('0') << setw(3) << loc_time.milli() << endl;
    /// 
    ///     return 0;
    /// }
    /// @endcode
    /// </p>
    /// @sa @c DateTimeSpan class, @c DateFuncs namespace
    /// 
	class MR_DATE_TIME_API DateTime : public boost::posix_time::ptime,
			private boost::equality_comparable<DateTime>,
			private boost::less_than_comparable<DateTime>
    {
    public:
        /// 
        /// Full weekday names ("Saturday", "Sunday" etc).
        /// 0 represents Saturday, 6 represents Friday.
        /// 
        static const char * const DAY[];

        /// 3-character abbreviated weekday names ("Sat", "Sun", etc).
        static const char * const DAY_ABBREV[];

        ///
        /// Full month names ("January", "February", etc).
        /// 1 represents January, 12 represents December (0 maps to an empty
        /// string).
        ///
        static const char * const MONTH[];

        /// 3-character abbreviated month names ("Jan", "Feb", etc).
        static const char * const MONTH_ABBREV[];

        ///
        /// Full season names ("Winter", "Spring", etc).
        /// 1 represents Winter, 4 represents Autumn (0 maps to an empty
        /// string).
        ///
        static const char * const SEASON[];

        ///
        /// Full Quarter names ("Q1", "Q2", etc).
        /// 1 represents Q1, 4 represents Q4 (0 maps to an empty
        /// string).
        ///
        static const char * const QUARTER[];

    public:
///////////////////////////////////////////////////////////////////////////////
////
//// "DateTime()" Creates a date-time object initialized to 19700101-000000.000
////
//// Author: Tim King, May 2016
//// group:DateTime
		explicit DateTime(const boost::posix_time::ptime& date_and_time = DT_TM_EPOCH);

		explicit DateTime(const boost::gregorian::date& dt);

		explicit DateTime(boost::posix_time::special_values special_date_time_value); 

        explicit DateTime(unsigned __int64 millis);

        explicit DateTime(double raw_dt_tm);

///////////////////////////////////////////////////////////////////////////////
////
//// "DateTime(unsigned long YYYYMMDD, unsigned long HHMMSS000)" Creates a date-time object initialized to the specified date and time
////
//// Author: Tim King, May 2016
//// group:DateTime
		DateTime(unsigned long date, unsigned long time);
		
///////////////////////////////////////////////////////////////////////////////
////
//// "DateTime(unsigned int yr, unsigned int long, unsigned int dy, unsigned int hr = 0, unsigned int min = 0, unsigned int sec = 0, unsigned int msec = 0)" Creates a date-time object initialized to the specified date and time
////
//// Author: Tim King, May 2016
//// group:DateTime
		DateTime(unsigned __int32 yr, 
                 unsigned __int32 mth, 
                 unsigned __int32 dy,
                 unsigned __int32 h = 0, 
                 unsigned __int32 m = 0, 
                 unsigned __int32 s = 0, 
                 unsigned __int32 ms = 0);

		static DateTime now();

///////////////////////////////////////////////////////////////////////////////
////
//// "bool DateTime::IsValid()" Return true if the DateTime holds a valid date and time.
////      
////    Author: Andrew Mart, March 2016
////    group:DateTime
		bool IsValid() const {return !IsNull();}

///////////////////////////////////////////////////////////////////////////////
////
//// "bool DateTime::IsNull()" Return true if the DateTime is uninitialised.
////    
////    Please note this replace the function DateTime::isNull() which has been deprecated
////    
////    Author: Tim King, May 2016
////    group:DateTime
		bool IsNull() const {return asPtime().is_not_a_date_time();}

///////////////////////////////////////////////////////////////////////////////
////
//// "bool DateTime::IsEpoch()" Return true if the DateTime is set to midnight, 1st January 1970
////    
////    Please note: this reproduces the behaviour of the function DateTime::isNull() which has been deprecated
////    
////    Author: Tim King, May 2016
////    group:DateTime
        bool IsEpoch() const {return asPtime() == DT_TM_EPOCH;}

///////////////////////////////////////////////////////////////////////////////
////
//// "static DateTime DateTime::Now()" Return the current date and time as a DateTime object
////    
////    For example
////        
////		Mat(0).Val("DateExec", DateTime::Now().Dt());
////    
////    group:DateTime
        static DateTime Now(){return DateTime::now();}

        static const DateTime Null;
        static const DateTime Epoch;
        static const double   NaN;

    typedef unsigned long YYYYMMDD;
    typedef unsigned long HHMMSS;

///////////////////////////////////////////////////////////////////////////////
////
//// "unsigned long DateTime::Dt() const" Return the date of the DateTime object as a "YYYYMMDD" numeric value
////    
////    For example
////        
////		FMat("DateExec", DateTime::Now().Dt());
////    
////    group:DateTime
        YYYYMMDD Dt() const {return getDate();}
///////////////////////////////////////////////////////////////////////////////
////
//// "unsigned long DateTime::Tm() const" Return the time of the DateTime object as a "HHMMSS" numeric value
////    
////    For example
////        
////		FMat("TimeExec", DateTime::Now().Tm());
////    
////    group:DateTime
        HHMMSS Tm() const {return getTime()/1000;}

		//Conversions
		static DateTime fromTime_t          (time_t t);
		static DateTime fromDatasetDouble   (double v);
		static DateTime fromMillis          (unsigned __int64 millis);

        static DateTime FromISOString       (const std::string& iso_string);
        static DateTime FromYYYYMMDD_hhmm   (const std::string& YYYYMMDD_hhmm);
        static DateTime FromYYYYMMDD_hhmmss (const std::string& YYYYMMDD_hhmmss);
        static DateTime FromDBString        (const std::string& YYYYMMDD_hhmmss000);

		time_t              toTime_t                () const;
		double              toDatasetDouble         () const;
        double              toYYYYMMDDhhmmssDouble  () const;
        double              toYYYYMMDDhhmmDouble    () const;
        double              toDouble                () const;
		unsigned __int64    toMillis                () const;

		//accessors
		ptime & asPtime() { return *this; }
		const ptime & asPtime() const { return *this; }

		static const boost::posix_time::ptime DT_TM_EPOCH;
        static const boost::posix_time::ptime DT_TM_NULL;

        unsigned __int32 year() const;
        unsigned __int32 month() const;
        unsigned __int32 day() const;
        unsigned __int32 dayOfWeek() const;
        unsigned __int32 hour() const;
        unsigned __int32 minute() const;
        unsigned __int32 sec() const;
        unsigned __int32 milli() const;

        unsigned __int32 season() const;
        unsigned __int32 quarter() const;

		unsigned __int64 secsSince1970() const;
		std::string getFormattedDate() const;

///////////////////////////////////////////////////////////////////////////////
////
//// "std::string DateTime::ToISOString() const" Return the DateTime as a "YYYYMMDDThhmmss.mmm" formatted string
////    
////    Author: Tim King, February 2016
////    
////    group:DateTime
        std::string DateTime::ToISOString() const;

///////////////////////////////////////////////////////////////////////////////
////
//// "std::string DateTime::ToYYYYMMDD_hhmm() const" Return the DateTime as a "YYYYMMDD-hhmm" formatted string
////    
////    Author: Tim King, February 2016
////    
////    group:DateTime
        std::string DateTime::ToYYYYMMDD_hhmm() const;

///////////////////////////////////////////////////////////////////////////////
////
//// "std::string DateTime::ToYYYYMMDD_hhmmss() const" Return the DateTime as a "YYYYMMDD-hhmmss" formatted string
////    
////    Author: Tim King, February 2016
////    
////    group:DateTime
        std::string DateTime::ToYYYYMMDD_hhmmss() const;

///////////////////////////////////////////////////////////////////////////////
////
//// "std::string DateTime::ToYYYYMMDD_hhmm() const" Return the DateTime as a "YYYYMMDD-hhmm" formatted string
////    
////    Author: Tim King, February 2016
////    
////    group:DateTime
        std::string DateTime::ToYYYYMMDDhhmm() const;

///////////////////////////////////////////////////////////////////////////////
////
//// "std::string DateTime::ToYYYYMMDD_hhmmss() const" Return the DateTime as a "YYYYMMDD-hhmmss" formatted string
////    
////    Author: Tim King, February 2016
////    
////    group:DateTime
        std::string DateTime::ToYYYYMMDDhhmmss() const;


///////////////////////////////////////////////////////////////////////////////
////
//// "std::string DateTime::ToDBString() const" Return the DateTime as a "YYYY-MM-DD hh:mm:ss.00" formatted string
////    
////    Author: Tim King, February 2016
////    
////    group:DateTime
        std::string DateTime::ToDBString() const;

        unsigned long getDate() const;
        void getDate(unsigned __int32 &yr, unsigned __int32 &mth, unsigned __int32 &dy) const;

        unsigned long getTime() const;
        void getTime(unsigned __int32 &h, unsigned __int32 &m, unsigned __int32 &s, unsigned __int32 &ms) const;

		//commands
        void setDate(unsigned long date);
        void setDate(unsigned __int32 yr, unsigned __int32 mth, unsigned __int32 dy);

        void setTime(unsigned long time = 0);
        void setTime(unsigned __int32 h, unsigned __int32 m, unsigned __int32 s = 0, unsigned __int32 ms = 0);

        void toLocalTime();
        void toGMT();
		DateTime asLocalTime() const;
		DateTime asGMT() const;

        bool isLeapYear() const;

        static unsigned __int32 daysInMonth(unsigned __int32 mth, unsigned __int32 yr);
        unsigned __int32 daysInMonth(unsigned __int32 mth) const;
        unsigned __int32 daysInMonth() const;

        DateTime &operator+=(const DateTimeSpan &span);
        DateTime &operator-=(const DateTimeSpan &span);

        DateTime operator+(const DateTimeSpan &span) const;
        DateTime operator-(const DateTimeSpan &span) const;

		DateTimeSpan operator-(const DateTime & other) const;

		bool operator==(const DateTime &other) const { return asPtime() == other.asPtime(); }
		bool operator<(const DateTime &other) const { return asPtime() < other.asPtime(); }

        static bool isValid(time_t t);
		static bool isValidYMD(unsigned __int32 yr, unsigned __int32 mth, unsigned __int32 dy);
		static bool isValidHMSms(unsigned __int32 h, unsigned __int32 m, unsigned __int32 s, unsigned __int32 ms);
		static bool isValidDate(unsigned long date_check);
		static bool isValidTime(unsigned long time_check);


    private:
		static bool isValidYear(unsigned __int32 year);
		static bool isValidMonth(unsigned __int32 month, unsigned __int32 year);
		static bool isValidHour(unsigned __int32 hour);
		static bool isValidMinute(unsigned __int32 minute);
		static bool isValidSecond(unsigned __int32 sec);
		static bool isValidMilliSecond(unsigned __int32 ms);

        DateTime& assign(time_t t);
    private:
		static const unsigned __int64 _MILLIS_PER_SEC = 1000;
		static const unsigned __int64 _SECS_PER_MIN = 60;
		static const unsigned __int64 _MINS_PER_HOUR = 60;
		static const unsigned __int64 _HOURS_PER_DAY = 24;
    };

    inline DateTime 
    DateTime::FromISOString(const std::string& iso_string)
    {
        return DateTime(boost::posix_time::from_iso_string(iso_string));
    }

    inline std::string 
    DateTime::ToISOString() const
    {
        return to_iso_string(*this);
    }

	inline DateTime
	DateTime::fromTime_t(time_t t)
	{ 
		return DateTime(DT_TM_EPOCH + boost::posix_time::seconds(static_cast<boost::uint32_t>(t)));
	}

	inline DateTime
	DateTime::fromDatasetDouble(double v) 
	{
		const boost::uint32_t* magic = reinterpret_cast<const boost::uint32_t*>(&v);
		++magic;
		return _isnan(v) ?
				DateTime(boost::posix_time::not_a_date_time)
				: fromTime_t(*magic);
	}

    inline double 
    DateTime::toYYYYMMDDhhmmssDouble() const
    {
	    return IsValid() 
            ?   10000.0 * year() +
		        100.0 * month() + 
		        day()   +  
		        0.01 * hour() +
		        0.0001 * minute() + 
		        0.000001 * sec()
            :   NaN;
    }

    inline double 
    DateTime::toYYYYMMDDhhmmDouble() const
    {
	    return IsValid() 
            ?   10000.0 * year() +
		        100.0 * month() + 
		        day()   +  
		        0.01 * hour() +
		        0.0001 * minute()
            :   NaN;
    }


}

MR_DATE_TIME_API std::ostream& operator<<(std::ostream& stream, const mr::DateTime& date_time);

///////////////////////////////////////////////////////////////////////////////

#endif /* __MRDATETIME_H__ */
