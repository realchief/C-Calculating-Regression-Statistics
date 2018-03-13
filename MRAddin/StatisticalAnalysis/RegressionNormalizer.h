#ifndef MR_STATISTICAL_ANALYSIS_REGRESSION_NORMALIZER_H
#define MR_STATISTICAL_ANALYSIS_REGRESSION_NORMALIZER_H

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
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP Normalize
{
public:
//  Lifetime:
    virtual    ~Normalize( ){}

//  Commands:
    virtual void setNormalizedResult( const RegressionResult& normalized_result ) = 0;

//  Queries:
    virtual double  operator()  ( double value ) const = 0;
    virtual double  invert      ( double value ) const = 0;

protected:
    Normalize( ){}
};
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP NormalizeNull : public Normalize
{
public:
//  Lifetime:
                NormalizeNull( ){}
    virtual    ~NormalizeNull( ){}

//  Commands:
    virtual void setNormalizedResult( const RegressionResult& ){}

//  Queries:
    virtual double  operator()  ( double value )    const {return value;}
    virtual double  invert      ( double value )    const {return value;}
};
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP NormalizeResponse : public Normalize
{
public:
//  Lifetime:
    explicit    NormalizeResponse( const RegressionResult& coarse_result );
    virtual    ~NormalizeResponse( ){}

//  Commands:
    virtual void setNormalizedResult( const RegressionResult& ){}

//  Queries:
    virtual double operator()( double value ) const
    {
        return (value - _mean) / _std_dev;
    }

    virtual double invert( double value ) const
    {
        return (_std_dev * value) + _mean;
    }

private:    
    double  _mean;
    double  _std_dev;
};
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP NormalizeIntercept : public Normalize
{
public:
//  Lifetime:
    explicit    NormalizeIntercept( const RegressionResult& coarse_result );
    virtual    ~NormalizeIntercept( ){}

//  Commands:
    virtual void setNormalizedResult( const RegressionResult& normalized_result );

//  Queries:
    virtual double  operator()  ( double predictor_value )  const {return predictor_value;}
    virtual double  invert      ( double beta  )            const;

private:    
    RawResults  _coarse_beta_factors;
    double      _response_mean;
    double      _response_std_dev;
    double      _normalized_beta;
};
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP NormalizePredictor : public Normalize
{
public:
//  Lifetime:
                NormalizePredictor( const RegressionResult& coarse_result,
                                     uint32_t predictor_index );
    virtual    ~NormalizePredictor( ){}

//  Commands:
    virtual void setNormalizedResult( const RegressionResult& ){}

//  Queries:
    virtual double  operator()  ( double value ) const;
    virtual double  invert      ( double value ) const;

private:    
    double      _response_std_dev;
    double      _predictor_mean;
    double      _predictor_std_dev;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace dataset_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_REGRESSION_NORMALIZER_H
////////////////////////////////////////////////////////////////////////////////
