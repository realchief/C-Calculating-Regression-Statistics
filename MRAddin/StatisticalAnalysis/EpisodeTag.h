#ifndef EPISODETAG_H_INCLUDED_20100409_171415_634064300559109795
#define EPISODETAG_H_INCLUDED_20100409_171415_634064300559109795

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

#include "DateTime/MRDateTime.h"

namespace mr
{
	namespace statistical_analysis
	{

#define COMPARE_RETURN_IF_DIFFERENT(A, B) if ((A) != (B)) return (A) < (B)

		class EpisodeTag
		{
		public:
			EpisodeTag(boost::int32_t what = -1,
					   const DateTime & begin_date_time = DateTime(),
					   const DateTime & end_date_time = DateTime()) :
				_what(what),
				_begin_date_time(begin_date_time),
				_end_date_time(end_date_time)//,
				//_episode_length((end_date_time.asPtime() - begin_date_time.asPtime()).total_milliseconds()),
				//_begin_time(_begin_date_time.time_of_day())
			{
			}

			boost::int32_t what() const { return _what; }
	
			const DateTime & beginDateTime() const { return _begin_date_time; }
			const DateTime & endDateTime() const { return _end_date_time; }

			/*bool operator < (const EpisodeTag & other) const
			{
				COMPARE_RETURN_IF_DIFFERENT(_what, other._what);
				COMPARE_RETURN_IF_DIFFERENT(_begin_time, other._begin_time);
				return _episode_length < other._episode_length;
			}*/

			bool operator == (const EpisodeTag & other) const
			{
				return _what == other._what && 
					   _begin_date_time == other._begin_date_time &&
					   _end_date_time == other._end_date_time;
			}

		private:
			boost::int32_t _what;
			DateTime _begin_date_time;
			DateTime _end_date_time;
			//boost::int32_t _episode_length;
			//boost::posix_time::time_duration _begin_time;
		};

		typedef std::vector<EpisodeTag> EpisodeTagVector;

		/*struct EpisodesVectorComparator : public std::binary_function<EpisodeTagVector, EpisodeTagVector, bool>
		{
			bool operator() (const EpisodeTagVector & left, const EpisodeTagVector & right) const
			{
				COMPARE_RETURN_IF_DIFFERENT(left.size(), right.size());
				return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
			}
		};*/
	}
}

#endif//EPISODETAG_H_INCLUDED_20100409_171415_634064300559109795
