#include "Common.h"
#include "StatisticalAnalysis/PredictedValueFunctor.h"
#include "StatisticalAnalysis/RegressionResult.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
PredictedValueFunctor::PredictedValueFunctor( const RegressionResult& regression )
:   _coefficients( regression.predictorCount() )
{
    Coefficients::iterator  coeff       = _coefficients.begin();
    RawResults::const_iterator beta     = regression.beta().begin();
    RawResults::const_iterator last     = regression.beta().end();
    PredictorMask::const_iterator mask  = regression.mask().begin();

    for( ; beta != last; ++beta, ++mask, ++coeff )
    {
        *coeff = *mask ? *beta : 0.0 ;
    }

}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
PredictedValueFunctor::~PredictedValueFunctor( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
//  Calculate the predicted value, which is the inner product of the 
/// regression coefficients and the predictor values.
///
inline double 
PredictedValueFunctor::operator()(const Vector& predictors) const
{
    DBC_REQUIRE( predictors.size() == _coefficients.size() );

    return inner_prod( _coefficients, predictors );
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
