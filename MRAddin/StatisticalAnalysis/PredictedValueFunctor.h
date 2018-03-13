#ifndef MR_STATISTICAL_ANALYSIS_PREDICTED_VALUE_FUNCTOR_H
#define MR_STATISTICAL_ANALYSIS_PREDICTED_VALUE_FUNCTOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP PredictedValueFunctor 
:   public std::unary_function<const Vector&, double>
{
    typedef Vector Coefficients;

public:
//  Lifetime:
    explicit  PredictedValueFunctor( const RegressionResult& regression );
             ~PredictedValueFunctor( );

//  Commands:

//  Functor:
    inline double operator()(const Vector& predictors) const;

private:

    Coefficients    _coefficients;

//  Not implemented:
    PredictedValueFunctor   ( const PredictedValueFunctor& );
    void operator=          ( const PredictedValueFunctor& );
};
////////////////////////////////////////////////////////////////////////////////

    }   // namespace statistical_analysis
}   // namespace mr

////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_PREDICTED_VALUE_FUNCTOR_H
////////////////////////////////////////////////////////////////////////////////
