#include "Common.h"
#include "StatisticalAnalysis/RegressionNormalizer.h"
#include "StatisticalAnalysis/RegressionResult.h"
#include "Infrastructure/ContainerTransform.h"
#include "Infrastructure/MathUtils.h"
#include "MRAlgorithm.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure::math;
using namespace infrastructure;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
bool isValidBeta( double beta )
{
    return !isNaN(beta) && _finite(beta);
}
////////////////////////////////////////////////////////////////////////////////

///
/// NormalizeResponse Constructor
///
NormalizeResponse::NormalizeResponse( const RegressionResult& coarse_result )
:   _mean   ( coarse_result.responseMean() ),
    _std_dev( coarse_result.responseStdDev() )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// NormalizeIntercept Constructor
///
NormalizeIntercept::NormalizeIntercept( const RegressionResult& coarse_result )
:   _coarse_beta_factors( coarse_result.predictorCount(), 
                          coarse_result.responseStdDev() ),
    _response_mean     ( coarse_result.responseMean() ),
    _response_std_dev  ( coarse_result.responseStdDev() ),
    _normalized_beta    ( NAN_NA )
{
//  Multiply each predictor mean by the response std dev
    fixedTransform(_coarse_beta_factors, coarse_result.predictorMeans(), 
        _coarse_beta_factors, boost::bind( multiplies<double>(), _1, _2 ) );    

//  Divide by predictor std dev
    fixedTransform(_coarse_beta_factors, coarse_result.predictorStdDevs(), 
        _coarse_beta_factors, boost::bind( divides<double>(), _1, _2 ) );    
}

void 
NormalizeIntercept::setNormalizedResult( const RegressionResult& normalized_result )
{
//  Multiply each coarse beta factor by the normalised betas
    fixedTransform(_coarse_beta_factors, normalized_result.beta(), 
        _coarse_beta_factors, boost::bind( multiplies<double>(), _1, _2 ) ); 

    RawResults betas;

//  Sum them (exclude predictors that have zero std-dev)
    copyIf(_coarse_beta_factors.begin(),_coarse_beta_factors.end(),
        back_inserter( betas ), 
            isValidBeta );

    _normalized_beta = std::accumulate(
        betas.begin(), betas.end(),
            -_response_mean );
}
////////////////////////////////////////////////////////////////////////////////
double  
NormalizeIntercept::invert( double beta  )const
{
    return beta * _response_std_dev - _normalized_beta;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
NormalizePredictor::NormalizePredictor( const RegressionResult& coarse_result,
                                          uint32_t predictor_index )
:   _response_std_dev  ( coarse_result.responseStdDev() ),
    _predictor_mean     ( coarse_result.predictorMean( predictor_index ) ),
    _predictor_std_dev  ( coarse_result.predictorStdDev( predictor_index ) )
{
}
double  
NormalizePredictor::operator()( double predictor_value ) const
{
    return (predictor_value != _predictor_mean) 
        ?   (predictor_value - _predictor_mean)/_predictor_std_dev
        :   0;
}

double  
NormalizePredictor::invert( double beta ) const
{
    return beta * _response_std_dev / _predictor_std_dev;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
