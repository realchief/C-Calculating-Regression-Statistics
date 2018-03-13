#include "StatisticalAnalysis/PartitionSequence.h"
#include "StatisticalAnalysis/SimultaneousEpisodes.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "DesignByContract.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template <typename ContainersConfiguration>
inline basic_gls_accumulator<ContainersConfiguration>::basic_gls_accumulator( uint32_t predictor_count )
:   basic_observation_accumulator<ContainersConfiguration>( predictor_count ),
    _accumulation           ( new basic_gls_accumulation<ContainersConfiguration>(predictor_count) )
{
	_accumulations.add(basic_gls_accumulation<ContainersConfiguration>::AccumulationPtr(_accumulation));

    _partitions.reset( new basic_partition_sequence<ContainersConfiguration>( *this ) );
}
////////////////////////////////////////////////////////////////////////////////

template <typename ContainersConfiguration>
inline void
basic_gls_accumulator<ContainersConfiguration>::accumulate(const basic_episode<ContainersConfiguration> * episode )
{
	_accumulation->accumulate( *episode ); 
}

///
/// Add the observation to the accumulation
///
template <typename ContainersConfiguration>
inline void 
basic_gls_accumulator<ContainersConfiguration>::add( const basic_episode<ContainersConfiguration> & episode )
{
    DBC_REQUIRE( episode.predictorCount() == _accumulations.predictorCount() );

    if( episode.isValid() )
    {
        _episodes.push_back( episode );
    }
}
///////////////////////////////////////////////////////////////////////////////

///
/// Set the current sample covariance matrix
///
template <typename ContainersConfiguration>
inline void
basic_gls_accumulator<ContainersConfiguration>::setInverseCovariance( const SymMatrix& inverse_covariance )
{
    _inverse_covariance = inverse_covariance;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Accumulate the batch of simultaneous episodes
///
template <typename ContainersConfiguration>
inline void 
basic_gls_accumulator<ContainersConfiguration>::accumulateEpisodeBatch( typename basic_simultaneous_episodes<ContainersConfiguration>::SimultaneousEpisodesPtr batch )
{
    DBC_REQUIRE( (*batch)().size() == _inverse_covariance.size1() );

    _accumulation->setBatchSize( (*batch)().size() );

    forEach((*batch)(), boost::bind(
        &basic_gls_accumulator<ContainersConfiguration>::accumulate, this, _1 ) );       

    _accumulation->accumulateBatch( _inverse_covariance );
}

template <typename ContainersConfiguration>
inline void 
basic_gls_accumulator<ContainersConfiguration>::accumulateEpisodesBatchVector(const typename basic_gls_accumulator<ContainersConfiguration>::Episodes & batch)
{
	DBC_REQUIRE( batch.size() == _inverse_covariance.size1() );

	_accumulation->setBatchSize(batch.size());

	infrastructure::forEach(batch, boost::bind(&basic_gls_accumulation<ContainersConfiguration>::accumulate, _accumulation, _1 ));       

	_accumulation->accumulateBatch( _inverse_covariance );
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
