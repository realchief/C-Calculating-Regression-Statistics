#ifndef MR_STATISTICAL_ANALYSIS_CONSERVE_MEAN_IMPUTER_H
#define MR_STATISTICAL_ANALYSIS_CONSERVE_MEAN_IMPUTER_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/Imputer.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP ConserveMeanImputer 
:   public Imputer
{
public:
//  Lifetime:
                ConserveMeanImputer( ){}
    virtual    ~ConserveMeanImputer( ){}

//  Commands:
    virtual void operator() ( Sample& sample );

//  Queries:

protected:
    void addObservation         ( const Vector& observation );
    void replaceMissingValues   ( Vector& observation, 
                                  const Vector& imputed_value ) const;

    Vector  _sum;
    Vector  _count;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_CONSERVE_MEAN_IMPUTER_H
////////////////////////////////////////////////////////////////////////////////
