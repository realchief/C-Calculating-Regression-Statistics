#ifndef MR_REGRESSION_OBSERVATION_ACCUMULATOR_H
#define MR_REGRESSION_OBSERVATION_ACCUMULATOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/AccumulationGroup.h"
#include "StatisticalAnalysis/Episode.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace boost;
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_observation_accumulator
{
public:
	typedef boost::shared_ptr<basic_observation_accumulator<ContainersConfiguration> > AccumulatorPtr;

    virtual ~basic_observation_accumulator( ) = 0{};

//  Abstract Commands:
    virtual void add( const basic_observation<ContainersConfiguration> & )  {DBC_UNREACHABLE;}
    virtual void add( const basic_episode<ContainersConfiguration> & )      {DBC_UNREACHABLE;}

//  Commands:
    void setAutoEMA     ( bool auto_ema );
    void setEMABoundary ( );
    void setEMALength   ( double ema_length );

//  Queries:
    uint64_t observationCount( ) const;
    uint64_t discardedCount  ( ) const;
    uint32_t predictorCount  ( ) const;
    uint64_t observationCount( uint32_t r ) const;

//  Navigable Associations
    const basic_accumulation_group<ContainersConfiguration>&    accumulations() const   {return _accumulations;}
    basic_accumulation_group<ContainersConfiguration>&          accumulations()         {return _accumulations;}

protected:
    explicit basic_observation_accumulator ( uint32_t num_predictors );

    bool    autoEMA         ( ) const {return _auto_ema;}

    basic_accumulation_group<ContainersConfiguration>   _accumulations;

    bool        _auto_ema;
    double      _ema_weight;
    uint64_t    _discarded;
};

typedef basic_observation_accumulator<DefaultContainers> ObservationAccumulator;

///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline uint64_t  
basic_observation_accumulator<ContainersConfiguration>::observationCount( ) const 
{
    return _accumulations.observationCount(0);
}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline uint64_t  
basic_observation_accumulator<ContainersConfiguration>::discardedCount( ) const 
{
    return _discarded;
}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline uint64_t  
basic_observation_accumulator<ContainersConfiguration>::observationCount( uint32_t a ) const 
{
    return _accumulations.observationCount(a);
}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline uint32_t  
basic_observation_accumulator<ContainersConfiguration>::predictorCount( ) const 
{
    return _accumulations.predictorCount();
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////

#include "ObservationAccumulator.inl"

#endif // MR_REGRESSION_OBSERVATION_ACCUMULATOR_H
///////////////////////////////////////////////////////////////////////////////