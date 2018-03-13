#include "Infrastructure/MathUtils.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace infrastructure;
using namespace infrastructure::math;
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template<typename ContainersConfiguration>
inline basic_episode<ContainersConfiguration>::basic_episode( const basic_observation<ContainersConfiguration> & observation,
                  double what,
                  double when,
                  double response_normalizer )
:   basic_observation<ContainersConfiguration>( observation ),
    _next                   ( 0 ),
    _previous               ( 0 ),
    _exclude_when_predictor ( false ),
    _exclude_when_response  ( false ),
    _index                  ( invalidIndex() ),
	_episode_tag(round<int32_t>(what), DateTime::fromDatasetDouble(when)),
	_dateTimeIsValid   ( false ),
	_snapshot(-1)
{
    DBC_REQUIRE( !isNaN(what) );
    DBC_REQUIRE( !isNaN(when) );

    setResponseNormalizer( response_normalizer );
}

template<typename ContainersConfiguration>
inline basic_episode<ContainersConfiguration>::basic_episode( const basic_observation<ContainersConfiguration> & observation,
                  double what,
                  const DateTime & begin_date_time,
                  double response_normalizer )
:   basic_observation<ContainersConfiguration>( observation ),
    _next                   ( 0 ),
    _previous               ( 0 ),
    _exclude_when_predictor ( false ),
    _exclude_when_response  ( false ),
    _index                  ( invalidIndex() ),
	_episode_tag(round<int32_t>(what), begin_date_time),
	_dateTimeIsValid   ( true )
{
    DBC_REQUIRE( !isNaN(what) );

    setResponseNormalizer( response_normalizer );
}

template<typename ContainersConfiguration>
inline basic_episode<ContainersConfiguration>::basic_episode( const basic_observation<ContainersConfiguration> & observation,
					 double what,
					 const DateTime & begin_date_time,
					 const DateTime & end_date_time)
:   basic_observation<ContainersConfiguration>( observation ),
    _next                   ( 0 ),
    _previous               ( 0 ),
    _exclude_when_predictor ( false ),
    _exclude_when_response  ( false ),
    _index                  ( invalidIndex() ),
	_episode_tag(round<int32_t>(what), begin_date_time, end_date_time),
	_dateTimeIsValid   ( false )
{
	setResponseNormalizer( 1.0 );
}

template<typename ContainersConfiguration>
inline basic_episode<ContainersConfiguration>::basic_episode( const basic_observation<ContainersConfiguration> & observation,
					 double what,
					 const DateTime & begin_date_time,
					 const DateTime & end_date_time,
					 double /*response_normalizer*/)
:   basic_observation<ContainersConfiguration>( observation ),
    _next                   ( 0 ),
    _previous               ( 0 ),
    _exclude_when_predictor ( false ),
    _exclude_when_response  ( false ),
    _index                  ( invalidIndex() ),
	_episode_tag(round<int32_t>(what), begin_date_time, end_date_time),
	_dateTimeIsValid   ( true )
{
	setResponseNormalizer( 1.0 );
}
////////////////////////////////////////////////////////////////////////////////

template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::updateIndex( const Digest& digest )
{    
	Digest::const_iterator what = digest.find( _episode_tag.what() );

	_index = (what != digest.end())
		? numeric_cast<uint32_t>(distance(digest.begin(),what))
		: invalidIndex();
}
////////////////////////////////////////////////////////////////////////////////

///
/// Returns an invalid key
///     Used within other methods to indicate error
///
template<typename ContainersConfiguration>
inline uint32_t 
basic_episode<ContainersConfiguration>::invalidIndex() 
{
	return std::numeric_limits<uint32_t>::max();
}

template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::buildLagSequence( basic_episode<ContainersConfiguration> * next )
{
    if( what() == next->what() )
    {
        DBC_ALWAYS_CHECK( next->when() > when() );

        setNext( next );

        next->setPrevious( this );
    }
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::useAllObservations( )
{
    _exclude_when_predictor = false;
    _exclude_when_response  = false;
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::useOnlyCommonPredictors( const Lags& lags )
{
    if( _exclude_when_predictor )return;

    Lags missing;

//  Back...
    transform(lags, missing, boost::bind(minus<int32_t>(), when(), _1) );

    findMissingDays<false>( missing );

    _exclude_when_predictor = !missing.empty();

    if( _exclude_when_predictor )return;   

//  Fore..
    transform(lags, missing, boost::bind(plus<int32_t>(), when(), _1 ) );

    findMissingDays<true>( missing );

    _exclude_when_predictor = !missing.empty();
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::useOnlyCommonResponses ( const Lags& lags )
{
    if( _exclude_when_response )return;

    Lags missing;

//  Back...
    transform(lags, missing, boost::bind(minus<int32_t>(), when(), _1) );

    findMissingDays<false>( missing );

    _exclude_when_response = !missing.empty();

    if( _exclude_when_response )return;   

//  Fore..
    transform(lags, missing, boost::bind(plus<int32_t>(), when(), _1 ) );

    findMissingDays<true>( missing );

    _exclude_when_response = !missing.empty();
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::useOnlyCommonPredictorsByRecord( const Lags& lags )
{
    if( _exclude_when_predictor )return;

    Lags missing;

//  Back...
    missing = lags;

    findMissingRecords<false>( missing, 0 );

    _exclude_when_predictor = !missing.empty();

    if( _exclude_when_predictor )return;   

//  Fore..
    missing = lags;

    findMissingRecords<true>( missing, 0 );

    _exclude_when_predictor = !missing.empty();
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_episode<ContainersConfiguration>::useOnlyCommonResponsesByRecord( const Lags& lags )
{
    if( _exclude_when_response )return;

//  Back...
    Lags missing = lags;

    findMissingRecords<false>( missing, 0 );

    _exclude_when_response = !missing.empty();

    if( _exclude_when_response )return;   

//  Fore..
    missing = lags;

    findMissingRecords<true>( missing, 0 );

    _exclude_when_response = !missing.empty();
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline const basic_episode<ContainersConfiguration> *  
basic_episode<ContainersConfiguration>::futureObservation( int32_t at ) const
{
//  Found the response required
    if( when() == at )
    {
        return this;
    }

//  Passed it
    if( when() > at )return 0;

//  No more observations
    if( !hasNext() )return 0;

//  Keep going    
    return _next->futureObservation(at);
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline const basic_episode<ContainersConfiguration> *  
basic_episode<ContainersConfiguration>::futureObservationByRecord( int32_t distance ) const
{
//  Found the response required
    if( distance == 0 )return this;

//  No more observations
    if( !hasNext() )return 0;

//  Keep going    
    return _next->futureObservationByRecord(--distance);
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline const basic_episode<ContainersConfiguration>*  
basic_episode<ContainersConfiguration>::pastObservation( int32_t at ) const
{
//  Found the response required
    if( when() == at )return this;

//  Passed by
    if( when()  < at )return 0;

//  No more observations
    if( !hasPrevious() )return 0;
        
//  Keep going    
    return _previous->pastObservation(at);
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline const basic_episode<ContainersConfiguration> *  
basic_episode<ContainersConfiguration>::pastObservationByRecord( int32_t distance ) const
{
//  Found the response required
    if( distance == 0 )return this;

//  No more observations
    if( !hasPrevious() )return 0;

//  Keep going    
    return _previous->pastObservationByRecord(--distance);
}
////////////////////////////////////////////////////////////////////////////////

    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
