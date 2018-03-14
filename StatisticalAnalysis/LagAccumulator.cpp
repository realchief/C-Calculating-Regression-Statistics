#include "Common.h"
#include "StatisticalAnalysis/LagAccumulator.h"
#include "StatisticalAnalysis/StdAccumulation.h"
#include "Infrastructure/MathUtils.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure::math;
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template<bool by_records> inline
LagAccumulator<by_records>::LagAccumulator( uint32_t predictor_count, 
                                            uint32_t response_count,
                                            int32_t lag )
:   ObservationAccumulator  ( predictor_count ),
    _lag                    ( lag )
{
    for( uint32_t r = 0; r < response_count; ++r )
    {
		_accumulations.add(StdAccumulation::AccumulationPtr(
            new StdAccumulation(predictor_count)));
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
template<bool by_records> inline
LagAccumulator<by_records>::~LagAccumulator(  )
{
}
///////////////////////////////////////////////////////////////////////////////

///
/// Add the observation to the accumulation
/// Reference: https://academic.oup.com/intqhc/article/23/3/331/1792904
/// Reference: http://www-stat.wharton.upenn.edu/~stine/mich/Lecture3.pdf
/// Reference: http://onlinelibrary.wiley.com/doi/10.1046/j.1365-2656.2003.00748.x/full
///
template<bool by_records> inline void
LagAccumulator<by_records>::add( const Episode& observation )
{

    if( !observation.isExcludedPredictor() && observation.isValid() )
    {
        const Episode* lagged_observation 
            = findLaggedObservation(observation);

        if( isNotNull(lagged_observation) 
        &&  !lagged_observation->isExcludedResponse() )
        {
            DBC_ALWAYS_CHECK( lagged_observation->what() == observation.what() );

            Response response(lagged_observation->response());

            if( !isNaN(response()) )
            {
                const double normalizer    
                    = observation.responseNormalizer();

                if( !isNaN(normalizer) && abs(normalizer) > 0 )
                {
                    response = response()/normalizer;

                    _accumulations.accumulate( 
                        Observation(observation.predictors(),response));

                    if (autoEMA())
                    {
                        setEMABoundary();
                    }
                }
            }
        }
    }

}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the records-based lagged observation
/// Reference: http://people.duke.edu/~rnau/regnotes.htm
/// Reference: https://stats.stackexchange.com/questions/25889/lagging-over-a-grouped-time-series
///
template< > inline const Episode* 
LagAccumulator<true>::findLaggedObservation( const Episode& observation )
{
    return (_lag >= 0)
        ?   observation.futureObservationByRecord( _lag )
        :   observation.pastObservationByRecord( -_lag );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the weekdays-based lagged observation
///
template< > inline const Episode* 
LagAccumulator<false>::findLaggedObservation( const Episode& observation )
{
    const int32_t at = observation.when() + _lag;

    return ( _lag >= 0 ) 
        ?   observation.futureObservation( at )
        :   observation.pastObservation( at );
}
////////////////////////////////////////////////////////////////////////////////
template class LagAccumulator<true>;
template class LagAccumulator<false>;
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
