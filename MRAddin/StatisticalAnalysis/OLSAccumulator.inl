#include "StatisticalAnalysis/Observation.h"
#include "StatisticalAnalysis/Episode.h"
#include "StatisticalAnalysis/StdAccumulation.h"
#include "DesignByContract.h"
#include "DebugNew.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline basic_ols_accumulator<ContainersConfiguration>::basic_ols_accumulator( uint32_t predictor_count )
:   basic_observation_accumulator<ContainersConfiguration>( predictor_count )
{
	boost::shared_ptr<basic_std_accumulation<ContainersConfiguration> > accumulation_ptr(new basic_std_accumulation<ContainersConfiguration>(predictor_count));
    _accumulations.add(accumulation_ptr);
}
///////////////////////////////////////////////////////////////////////////////

///
/// Add the observation to the accumulation
///
template <typename ContainersConfiguration>
inline void 
basic_ols_accumulator<ContainersConfiguration>::add( const basic_observation<ContainersConfiguration> & observation )
{
	if (!observation.isValid())
	{
        ++_discarded;
		return;
	}

    DBC_REQUIRE( observation.predictorCount() == _accumulations.predictorCount() );

	_accumulations.accumulate( observation );

    if (autoEMA())
    {
        setEMABoundary();
    }
}
///////////////////////////////////////////////////////////////////////////////

///
/// Add the observation to the accumulation
///
template <typename ContainersConfiguration>
inline void 
basic_ols_accumulator<ContainersConfiguration>::add( const basic_episode<ContainersConfiguration>& observation )
{
    DBC_REQUIRE( observation.predictorCount() == _accumulations.predictorCount() );

    if( observation.isValid() )
    {
        _accumulations.accumulate( observation );

        if (autoEMA())
        {
            setEMABoundary();
        }
    }
    else
    {
        ++_discarded;
    }
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analyis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////