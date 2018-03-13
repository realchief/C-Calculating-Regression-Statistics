#ifndef MR_STATISTICAL_ANALYSIS_IMPUTER_H
#define MR_STATISTICAL_ANALYSIS_IMPUTER_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP Imputer
{
public:
//  Lifetime:
    virtual    ~Imputer( ){}

//  Commands:
    virtual void operator() ( Sample& sample ) = 0;

//  Queries:

protected:
//  Lifetime:
    Imputer( ){}
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_IMPUTER_H
////////////////////////////////////////////////////////////////////////////////