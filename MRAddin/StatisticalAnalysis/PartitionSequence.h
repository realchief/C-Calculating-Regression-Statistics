#ifndef MR_STATISTICAL_ANALYSIS_PARTITION_SEQUENCE_H
#define MR_STATISTICAL_ANALYSIS_PARTITION_SEQUENCE_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "Infrastructure/LazySet.h"
#include "StatisticalAnalysis/Episode.h"
#include "EpisodePartition.h"

////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
using namespace std;
using namespace infrastructure;

	template <typename ContainersConfiguration> class basic_gls_accumulator;
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
class basic_partition_sequence
{
    typedef lazy_set<double>            Digest;
	typedef vector<typename basic_episode_partition<ContainersConfiguration>::EpisodePartitionPtr> Partitions;
	typedef basic_partition_sequence<ContainersConfiguration> MyType;

public:
	typedef boost::shared_ptr<MyType> PartitionsPtr;
//  Lifetime:
    explicit basic_partition_sequence( basic_gls_accumulator<ContainersConfiguration> & accumulator );
            ~basic_partition_sequence( );

//  Commands:
    void create     ( );
    void accumulate ( );

//  Queries:
    uint32_t batchCount     ( ) const {return _batch_count;}
    uint32_t partitionCount ( ) const {return _partition_count;}

//  Callback:
    void setCallbacks( ProgressSink partition_sink,
                       ProgressSink batch_sink,
                       CancelQuery cancel_query );

private:
//  Navigable Associations:
    const Partitions& partitions( ) const {return _partitions;}

    void accumulatePartition    ( typename basic_episode_partition<ContainersConfiguration>::EpisodePartitionPtr partition,
                                  uint32_t& partion_progress,
                                  uint32_t& batch_progress );

//  Associations:
    basic_gls_accumulator<ContainersConfiguration> & _accumulator;
    Digest          _digest;
    Partitions      _partitions;

//  Callback:
    ProgressSink _partition_progress;
    ProgressSink _batch_progress;
    CancelQuery  _cancel_query;

//  Attributes:
    uint32_t _batch_count;
    uint32_t _partition_count;

//  Not Implemented
    basic_partition_sequence( );
    basic_partition_sequence( const MyType & );
    MyType & operator=( const MyType& );
};

typedef basic_partition_sequence<DefaultContainers> PartitionSequence;
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr

#include "PartitionSequence.inl"

////////////////////////////////////////////////////////////////////////////////
#endif // MR_GLS_SAMPLE_H
////////////////////////////////////////////////////////////////////////////////
