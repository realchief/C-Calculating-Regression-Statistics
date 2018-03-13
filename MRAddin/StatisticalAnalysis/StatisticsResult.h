#ifndef MR_STATISTICAL_ANALYSIS_STATISTICS_RESULT_H
#define MR_STATISTICAL_ANALYSIS_STATISTICS_RESULT_H

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
class STATISTICAL_ANALYSIS_EXPIMP StatisticsResult
{
public:
                    StatisticsResult        ( );
    explicit        StatisticsResult        ( double value );
                    StatisticsResult        ( double value, double sig );

    bool            hasValidSignificance    ( ) const;
    virtual bool    isValid                 ( ) const;
    double          value                   ( ) const { return _value; }
    double          mrSignificance          ( ) const { return _mr_significance; }
    double          significance            ( ) const { return _significance; }

protected:
    void            setSignificance         ( double significance );

    double          _value;
    double          _mr_significance;
    double          _significance;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif  // MR_STATISTICAL_ANALYSIS_STATISTICS_RESULT_H
////////////////////////////////////////////////////////////////////////////////
