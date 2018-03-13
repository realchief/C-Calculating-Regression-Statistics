#ifndef MR_STATISTICAL_ANALYSIS_CORRELATION_RESULT_H
#define MR_STATISTICAL_ANALYSIS_CORRELATION_RESULT_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/StatisticsResult.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
        using boost::uint64_t;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP CorrelationResult : public StatisticsResult
{
public:
    CorrelationResult   ( );
    CorrelationResult   ( double correlation,
                          uint64_t count );
    CorrelationResult   ( double correlation, 
                          double significance,
                          uint64_t count );

//  Using compiler generated copy constructor and assignment operator

//  Queries:
    double          ciLowerBound( ) const {return _ci_lower_bound;}
    double          ciUpperBound( ) const {return _ci_upper_bound;}
    double          correlation ( ) const {return value();}
    virtual bool    isValid     ( ) const;

private:
    double      _ci_lower_bound;     // 95% Confidence interval is not symmetric
    double      _ci_upper_bound;     // 95% Confidence interval is not symmetric
    uint64_t    _count;

    void    calculateSignificance       ( );
    void    calculateConfidenceInterval ( );
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif  // MR_STATISTICAL_ANALYSIS_CORRELATION_RESULT_H
////////////////////////////////////////////////////////////////////////////////
