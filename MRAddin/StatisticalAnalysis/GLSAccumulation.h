#ifndef MR_GLS_ACCUMULATION_H
#define MR_GLS_ACCUMULATION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/Accumulation.h"
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/SwitchedContainersConfig.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace boost;
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
class basic_gls_accumulation : public basic_accumulation<ContainersConfiguration>
{
public:

//  Lifetime
    explicit basic_gls_accumulation( uint32_t predictor_count );
             basic_gls_accumulation( );
    virtual ~basic_gls_accumulation( ){}

//  Commands:
    void setBatchSize       ( size_t batch_size );
    void accumulateBatch    ( const SymMatrix& covariance_inverse );
        
//  Concrete Commands:
    virtual void accumulate ( const basic_observation<ContainersConfiguration> & observation );

private:
    typename ContainersConfiguration::matrix_type _X;
    Vector  _Y;
    size_t  _batch_size;
    size_t  _observation_k;
};

typedef basic_gls_accumulation<DefaultContainers> GLSAccumulation;
///////////////////////////////////////////////////////////////////////////////
    }
}

#include "GLSAccumulation.inl"

///////////////////////////////////////////////////////////////////////////////
#endif //   MR_GLS_ACCUMULATION_H
///////////////////////////////////////////////////////////////////////////////
