#ifndef MR_GLS_ACCUMULATOR_H
#define MR_GLS_ACCUMULATOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/ObservationAccumulator.h"
#include "StatisticalAnalysis/Episode.h"
#include "StatisticalAnalysis/GLSAccumulation.h"
#include "StatisticalAnalysis/Sample.h"
#include "StatisticalAnalysis/API.h"
#include "SimultaneousEpisodes.h"
#include "PartitionSequence.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
class basic_gls_accumulator : public basic_observation_accumulator<ContainersConfiguration>
{
	typedef typename basic_partition_sequence<ContainersConfiguration>::PartitionsPtr PartitionsPtr;
	typedef basic_gls_accumulator<ContainersConfiguration> MyType;
public:
	typedef boost::shared_ptr<MyType> GLSAccumulatorPtr;
    typedef Matrix Covariance;
	typedef std::vector<basic_episode<ContainersConfiguration> > Episodes;

//  Lifetime:
    explicit basic_gls_accumulator( uint32_t predictor_count );
	virtual ~basic_gls_accumulator( ) {};

//  Navigable Associations:
    const Episodes& episodes    ( ) const   {return _episodes;}
    Episodes&       episodes    ( )         {return _episodes;}
    PartitionsPtr   partitions  ( )         {return _partitions;}

//  Concrete Commands:
    virtual void add( const basic_episode<ContainersConfiguration> & episode );

//  Commands:
    void setInverseCovariance   ( const SymMatrix& inverse_covariance );
	void accumulateEpisodeBatch ( typename basic_simultaneous_episodes<ContainersConfiguration>::SimultaneousEpisodesPtr batch );
	void accumulateEpisodesBatchVector( const Episodes & batch );

protected:

	void accumulate ( const basic_episode<ContainersConfiguration> * episode );

	basic_gls_accumulation<ContainersConfiguration> * _accumulation;
    Episodes            _episodes;
    PartitionsPtr       _partitions;
    SymMatrix           _inverse_covariance;
};
////////////////////////////////////////////////////////////////////////////////
typedef basic_gls_accumulator<DefaultContainers> GLSAccumulator;

////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr

#include "GLSAccumulator.inl"

////////////////////////////////////////////////////////////////////////////////
#endif // MR_GLS_ACCUMULATOR_H
////////////////////////////////////////////////////////////////////////////////
