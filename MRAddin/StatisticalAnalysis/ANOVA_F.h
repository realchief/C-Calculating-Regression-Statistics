#ifndef MR_STATISTICAL_ANALYSIS_ANOVA_F_H
#define MR_STATISTICAL_ANALYSIS_ANOVA_F_H

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
///////////////////////////////////////////////////////////////////////////////
        using boost::uint32_t;
///////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP ANOVA_F
{
public:
            ANOVA_F( );
           ~ANOVA_F( );

//  Commands:
    void addGroup           ( uint32_t count, 
                              double sum, 
                              double sum_sq );

    void addGroup           ( const PopulationSummary& population );
    void addGroupSynopsis   ( const Synopsis& sample );

    void calculateResult    ( );
    void calculateResult    ( uint32_t total_count, 
                              double total_sum );


//  Queries:
    const uint32_t          groupCount  ( ) const {return _group_count;}
    const StatisticsResult& result      ( ) const {return _result;}

private:
    uint32_t            _group_count;
    StatisticsResult    _result;
    double              _sum_sq_between;
    double              _sum_sq_within;
    uint32_t            _total_count;
    double              _total_sum;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_ANOVA_F_H
///////////////////////////////////////////////////////////////////////////////