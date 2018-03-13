#ifndef MR_STATISTICAL_ANALYSIS_EPISODE_PARTITION_H
#define MR_STATISTICAL_ANALYSIS_EPISODE_PARTITION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/SimultaneousEpisodes.h"
#include "StatisticalAnalysis/SortPredicates.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "Infrastructure/LazySet.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace infrastructure;
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
class basic_episode_partition
{
    typedef basic_episode_partition<ContainersConfiguration> MyType;

	typedef lazy_set<double>                                Digest;
    typedef vector<double>                                  Key;
	typedef boost::shared_ptr<basic_simultaneous_episodes<ContainersConfiguration> > SimultaneousEpisodesPtr;
    typedef lazy_set<SimultaneousEpisodesPtr, 
                     PartitionPred<SimultaneousEpisodesPtr> > Batches;
	
public:
	typedef boost::shared_ptr<MyType> EpisodePartitionPtr;

//  Lifetime:
    explicit    basic_episode_partition( SimultaneousEpisodesPtr episodes );
               ~basic_episode_partition( );

//  Navigable Associations:
    const Key&      key         ( ) const {return _key;}
    const Batches&  operator()  ( ) const {return _batches;}
    const Digest&   digest      ( ) const {return _digest;}

//  Commands:
    void include( SimultaneousEpisodesPtr episodes );

//  Queries:
    bool    canInclude          ( SimultaneousEpisodesPtr episodes )const;
    SymMatrix  covarianceInverse   ( ) const;

private:

//  Associations:
    Batches _batches;
    Key     _key;
    Digest  _digest;

//  Not implemented:
    basic_episode_partition ( );
};

typedef basic_episode_partition<DefaultContainers> EpisodePartition;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr

#include "EpisodePartition.inl"

////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_EPISODE_PARTITION_H
////////////////////////////////////////////////////////////////////////////////
