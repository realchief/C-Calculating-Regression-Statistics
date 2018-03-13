#ifndef MR_OLS_ACCUMULATOR_H
#define MR_OLS_ACCUMULATOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/ObservationAccumulator.h"
#include "StatisticalAnalysis/AccumulationGroup.h"
#include "StatisticalAnalysis/API.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////

template <typename ContainersConfiguration>
class basic_ols_accumulator : public basic_observation_accumulator<ContainersConfiguration>
{
public:
    explicit basic_ols_accumulator( uint32_t predictor_count );
    virtual ~basic_ols_accumulator( ){}

//  Commands:    
    virtual void add( const basic_observation<ContainersConfiguration> & observation );
    virtual void add( const basic_episode<ContainersConfiguration> & observation );

};

typedef basic_ols_accumulator<DefaultContainers> OLSAccumulator;
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////

#include "OLSAccumulator.inl"

#endif // MR_OLS_ACCUMULATOR_H
///////////////////////////////////////////////////////////////////////////////