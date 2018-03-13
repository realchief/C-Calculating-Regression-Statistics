#ifndef MR_STATISTICAL_ANALYSIS_EPISODE_H
#define MR_STATISTICAL_ANALYSIS_EPISODE_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/Observation.h"
#include "StatisticalAnalysis/EpisodeTag.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_episode : public basic_observation<ContainersConfiguration>
{
public:
//  Lifetime:
    basic_episode  ( const basic_observation<ContainersConfiguration> & observation,
					 double what,
					 double when,
					 double response_normalizer = 1.0 );

	basic_episode  ( const basic_observation<ContainersConfiguration> & observation,
					 double what,
					 const DateTime & begin_date_time,
					 double response_normalizer = 1.0 );

	basic_episode  ( const basic_observation<ContainersConfiguration> & observation,
					 double what,
					 const DateTime & begin_date_time,
					 const DateTime & end_date_time);

	basic_episode  ( const basic_observation<ContainersConfiguration> & observation,
					 double what,
					 const DateTime & begin_date_time,
					 const DateTime & end_date_time,
					 double response_normalizer);

	~basic_episode  ( ) {};

//  Commands:
    void updateIndex                    ( const Digest& digest );
    void useAllObservations             ( );
    void useOnlyCommonPredictors        ( const Lags& lags );
    void useOnlyCommonResponses         ( const Lags& lags );
    void useOnlyCommonPredictorsByRecord( const Lags& lags );
    void useOnlyCommonResponsesByRecord ( const Lags& lags );

	void setSnapshot(int snapshot)
	{
		DBC_REQUIRE(snapshot>=0 && snapshot<=100);
		_snapshot = snapshot;
	}

//  Queries:
    uint32_t    index               ( ) const   {return _index;}
    bool        isExcludedPredictor ( ) const   {return _exclude_when_predictor;}
    bool        isExcludedResponse  ( ) const   {return _exclude_when_response;}
    int32_t     what                ( ) const   {return tag().what();}
	int32_t     when                ( ) const   
	{
		if ( !_dateTimeIsValid )
		{
			return round<int32_t>(tag().beginDateTime().toDatasetDouble());
		}
		else
		{
			return round<int32_t>(tag().beginDateTime().secsSince1970()/86400.0);
		}
	}
	int snapshot() const { return _snapshot; }

	const EpisodeTag & tag() const { return _episode_tag; }
	bool beginDateTimeIsValid() const { return _dateTimeIsValid; }


//  Doubly-Linked List
    void buildLagSequence( basic_episode<ContainersConfiguration>* next );

//  Observation search:
    const basic_episode<ContainersConfiguration> * futureObservation           ( int32_t at )          const;
    const basic_episode<ContainersConfiguration> * futureObservationByRecord   ( int32_t distance )    const;
    const basic_episode<ContainersConfiguration> * pastObservation             ( int32_t at )          const;
    const basic_episode<ContainersConfiguration> * pastObservationByRecord     ( int32_t distance )    const;

	static uint32_t	invalidIndex();   

private:

//  Doubly-Linked List
    void setNext    ( basic_episode<ContainersConfiguration> * next )    {_next = next;}
    void setPrevious( basic_episode<ContainersConfiguration> * previous ){_previous = previous;}

    template<bool forward>
    void findMissingDays    ( Lags& unavailable_lags ) const;
	
	template<>
	void findMissingDays<true>( Lags& missing ) const
	{
		if( when() > *missing.rbegin() )return;

		missing.erase( when() );

		if( !missing.empty() && hasNext() )
		{
			_next->findMissingDays<true>( missing );
		}
	}
	template<>
	void findMissingDays<false>( Lags& missing ) const
	{
		if( when() < *missing.begin() )return;

		missing.erase( when() );

		if( !missing.empty() && hasPrevious() )
		{
			_previous->findMissingDays<false>( missing );
		}
	}


	
    template<bool forward>
    void findMissingRecords ( Lags& unavailable_lags, 
                              int32_t distance ) const;

	template<>
	void findMissingRecords<false>( Lags& missing, int32_t distance ) const
	{
		missing.erase( distance );

		if( !missing.empty() && hasPrevious() )
		{
			_previous->findMissingRecords<false>( missing, ++distance );
		}
	}

	template<>
	void findMissingRecords<true>(  Lags& missing, int32_t distance ) const
	{
		missing.erase( distance );

		if( !missing.empty() && hasNext() )
		{
			_next->findMissingRecords<true>( missing, ++distance );
		}
	}

//  Helpers
    bool hasNext    ( ) const   {return isNotNull(_next);}
    bool hasPrevious( ) const   {return isNotNull(_previous);}

//  Associations:
    basic_episode<ContainersConfiguration> * _next;
    basic_episode<ContainersConfiguration> * _previous;

//  Implementation:
    bool        _exclude_when_predictor;
    bool        _exclude_when_response;
    uint32_t    _index;
    
	EpisodeTag _episode_tag;
	bool		_dateTimeIsValid;
	int         _snapshot;
};

typedef basic_episode<DefaultContainers> Episode;
typedef infrastructure::lazy_set<Episode*, SimultaneousPred<Episode*> > EpisodesSet;

//template <typename ContainersConfiguration>
//inline basic_episode<ContainersConfiguration> * toPtr( basic_episode<ContainersConfiguration> & o ){return &o;}
inline Episode * toPtr( Episode & o ){return &o;}

//template <typename ContainersConfiguration>
//inline basic_episode<ContainersConfiguration> & toRef( basic_episode<ContainersConfiguration> * o ){return *o;}
inline Episode & toRef( Episode * o ){return *o;}

////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////

#include "Episode.inl"

#endif // MR_STATISTICAL_ANALYSIS_EPISODE_H
////////////////////////////////////////////////////////////////////////////////
