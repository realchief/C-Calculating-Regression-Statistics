#ifndef MR_REGRESSION_RESULT_BINDER_H
#define MR_REGRESSION_RESULT_BINDER_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/LinearRegression.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP RegressionResultBinder
{
public:
	typedef boost::shared_ptr<LinearRegression> LinearRegressionPtr;
//  Lifetime:
    RegressionResultBinder  ( LinearRegressionPtr regression, 
                              bool multi_variate );
            ~RegressionResultBinder ( );

//  Navigable Associations:
    LinearRegressionPtr         regression  ( ) const {return _regression;}
    const RegressionResults&    results     ( ) const {return _results;}   

//  Commands:
    void appendResults( const RegressionMask& mask );

//  Queries:
    bool    multiVariate() const {return _multi_variate;}
private:

//  Associations:
    LinearRegressionPtr _regression;
    RegressionResults   _results;
//  Attributes:
    bool    _multi_variate;

//  Not implemented:
    RegressionResultBinder  ( );
    RegressionResultBinder  ( const RegressionResultBinder& );
    void operator=          ( const RegressionResultBinder& );
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_REGRESSION_RESULT_BINDER_H
////////////////////////////////////////////////////////////////////////////////
