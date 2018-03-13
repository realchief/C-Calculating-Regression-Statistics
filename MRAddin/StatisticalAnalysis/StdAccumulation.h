#ifndef MR_STATISTICAL_ANALYSIS_STANDARD_ACCUMULATION_H
#define MR_STATISTICAL_ANALYSIS_STANDARD_ACCUMULATION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/Accumulation.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost;
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_std_accumulation : public basic_accumulation<ContainersConfiguration>
{
public:
//  Lifetime
    explicit    basic_std_accumulation ( uint32_t predictor_count );
                basic_std_accumulation ( );
    virtual    ~basic_std_accumulation ( ){}

//  Implemented Commands
    virtual void accumulate( const basic_observation<ContainersConfiguration> & observation );
};

typedef basic_std_accumulation<DefaultContainers> StdAccumulation;
///////////////////////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////////////////////

#include "StdAccumulation.inl"

#endif //   MR_STATISTICAL_ANALYSIS_STANDARD_ACCUMULATION_H
///////////////////////////////////////////////////////////////////////////////
