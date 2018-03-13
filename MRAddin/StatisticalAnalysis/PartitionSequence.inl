#include "StatisticalAnalysis/SimultaneousEpisodes.h"
#include "StatisticalAnalysis/EpisodePartition.h"
#include "StatisticalAnalysis/GLSAccumulator.h"
#include "StatisticalAnalysis/Sample.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "Infrastructure/Loop.h"
#include "Infrastructure/DummyProgress.h"
#include "Infrastructure/DummyCancel.h"
#include "DesignByContract.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template<typename ContainersConfiguration>
inline basic_partition_sequence<ContainersConfiguration>::basic_partition_sequence( basic_gls_accumulator<ContainersConfiguration> & accumulator )
:   _accumulator        (accumulator),
	_partition_progress (infrastructure::DummyProgress()),
	_batch_progress     (infrastructure::DummyProgress()),
	_cancel_query	    (infrastructure::DummyCancel()),
    _batch_count        (0),
    _partition_count    (0)
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
template<typename ContainersConfiguration>
inline basic_partition_sequence<ContainersConfiguration>::~basic_partition_sequence()
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Partition the episodes by sorting them into coincident observations
///
template<typename ContainersConfiguration>
inline void
basic_partition_sequence<ContainersConfiguration>::create( )
{
	typedef basic_simultaneous_episodes<ContainersConfiguration>::Episodes RawSequence;
	typedef basic_simultaneous_episodes<ContainersConfiguration>::SimultaneousEpisodesPtr SimultaneousEpisodesPtr;

    RawSequence sequence;

    forEach( _accumulator.episodes(), boost::bind( 
        &RawSequence::fast_insert, &sequence, 
            boost::bind( toPtr, _1 ) ) );

    RawSequence::const_iterator episode 
        = sequence.begin();

    typedef vector <SimultaneousEpisodesPtr> Batches;

    Batches batches;

    batches.push_back( 
        SimultaneousEpisodesPtr( 
            new SimultaneousEpisodes( 
                *episode++ ) ) );

    for( RawSequence::const_iterator last_episode = sequence.end(); 
         episode != last_episode; ++episode )
    {
        if( batches.back()->canInclude( *episode ) )
        {
            batches.back()->include( *episode );
        }
        else
        {
            batches.push_back( SimultaneousEpisodesPtr( 
                new SimultaneousEpisodes( *episode ) ) );
        }
        _digest.fast_insert((*episode)->what());
    }

    forEach( batches, boost::bind( 
        &SimultaneousEpisodes::buildKey, _1, 
            boost::cref(_digest) ) );

    PartitionPred<SimultaneousEpisodesPtr> partition_sort;

    std::sort( batches.begin(), batches.end(), 
        partition_sort );

    Batches::const_iterator batch = batches.begin();
    Batches::const_iterator last_batch = batches.end();

	_partitions.push_back( typename basic_episode_partition<ContainersConfiguration>::EpisodePartitionPtr( 
        new basic_episode_partition<ContainersConfiguration>( *batch++ ) ) );

    for( ; batch != last_batch; ++batch )
    {
        if( _partitions.back()->canInclude( *batch ) )
        {
            _partitions.back()->include( *batch );
        }
        else
        {
            _partitions.push_back( typename basic_episode_partition<ContainersConfiguration>::EpisodePartitionPtr( 
                new basic_episode_partition<ContainersConfiguration>( *batch ) ) );
        }
    }

    _batch_count        = numeric_cast<uint32_t>(batches.size());
    _partition_count    = numeric_cast<uint32_t>(_partitions.size());
}
////////////////////////////////////////////////////////////////////////////////

///
/// Feed the partitions, one at a time, into the accumulator
///
template<typename ContainersConfiguration>
inline void
basic_partition_sequence<ContainersConfiguration>::accumulate( )
{
    uint32_t partition_progress = 0;
    uint32_t batch_progress     = 0;

    forEachUntil( _partitions, boost::bind(
        &basic_partition_sequence<ContainersConfiguration>::accumulatePartition, this, _1, 
            boost::ref(partition_progress),
                boost::ref(batch_progress) ),
                    _cancel_query );
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_partition_sequence<ContainersConfiguration>::accumulatePartition( typename basic_episode_partition<ContainersConfiguration>::EpisodePartitionPtr partition,
                                        uint32_t& partition_progress,
                                        uint32_t& batch_progress )
{
    _accumulator.setInverseCovariance(partition->covarianceInverse());

	forEach( (*partition)(), boost::bind(
        &basic_gls_accumulator<ContainersConfiguration>::accumulateEpisodeBatch, 
            &_accumulator, _1 ) );

    batch_progress    += numeric_cast<uint32_t>((*partition)().size());
    partition_progress  += 1;

	_batch_progress  ( batch_progress, std::string() ); 
    _partition_progress( partition_progress, std::string() );
}
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void
basic_partition_sequence<ContainersConfiguration>::setCallbacks( ProgressSink partition_sink,
                         ProgressSink batch_sink,
                         CancelQuery cancel_query )
{
    _partition_progress = (partition_sink != 0)
        ? partition_sink 
		: infrastructure::DummyProgress();

    _batch_progress = (batch_sink != 0)
        ? batch_sink 
        : infrastructure::DummyProgress();

    _cancel_query = (cancel_query != 0) 
        ? cancel_query 
        : infrastructure::DummyCancel();
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
