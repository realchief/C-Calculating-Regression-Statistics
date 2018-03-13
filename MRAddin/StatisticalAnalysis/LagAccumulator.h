#ifndef MR_LAG_ACCUMULATOR_H
#define MR_LAG_ACCUMULATOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/ObservationAccumulator.h"
#include "StatisticalAnalysis/Episode.h"
#include "StatisticalAnalysis/API.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
template<bool by_records>
class STATISTICAL_ANALYSIS_EXPIMP LagAccumulator : public ObservationAccumulator
{
public:
//  Lifetime:
                LagAccumulator( uint32_t predictor_count,
                                uint32_t response_count,
                                int32_t lag );
    virtual    ~LagAccumulator( );

//  Concrete Commands:
    virtual void add( const Episode& observation );

//  Commands:

protected:
//  Implementation:
    int32_t _lag;

    const Episode* findLaggedObservation( const Episode& observation );

private:
    LagAccumulator  ( );
    LagAccumulator  ( const LagAccumulator& );
    void operator=  ( const LagAccumulator& );
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_LAG_ACCUMULATOR_H
////////////////////////////////////////////////////////////////////////////////
