#ifndef MR_STATISTICAL_ANALYSIS_ANOVA_T_H
#define MR_STATISTICAL_ANALYSIS_ANOVA_T_H

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
class STATISTICAL_ANALYSIS_EXPIMP ANOVA_t
{
public:
    ANOVA_t( uint32_t total_count, 
             double total_sum, 
             double total_sum_sq );

   ~ANOVA_t( );

//  Queries:
    StatisticsResult    multipleGroupResult ( uint32_t local_count, 
                                              double local_sum, 
                                              double local_sum_sq,
                                              double local_std_dev,
                                              double local_std_err ) const;

    StatisticsResult    singleGroupResult   ( uint32_t local_count, 
                                              double local_sum, 
                                              double local_std_dev,
                                              double local_std_err ) const;
private:
    uint32_t    _total_count;
    double      _total_sum;
    double      _total_sum_sq;

};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_ANOVA_T_H
///////////////////////////////////////////////////////////////////////////////