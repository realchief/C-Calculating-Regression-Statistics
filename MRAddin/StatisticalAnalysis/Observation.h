#ifndef MR_STATISTICAL_ANALYSIS_OBSERVATION_H
#define MR_STATISTICAL_ANALYSIS_OBSERVATION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/Response.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/PredictedValueFunctor.h"
#include "Infrastructure/MathUtils.h"
#include "StatisticalAnalysis/NumericContainersConfiguration.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_observation
{
public:
//  Lifetime:
	explicit    basic_observation( Values::size_type predictor_count );

                basic_observation( const typename ContainersConfiguration::vector_type & predictors,
                             const Response& response );

                basic_observation( const typename ContainersConfiguration::vector_type & predictors,
                             double response );

                basic_observation( const Values& predictors,
                             double response );

				virtual    ~basic_observation( ) {};

//  USING COMPILER GENERATED COPY CONSTRUCTOR and ASSIGNMENT OPERATOR

//  Navigable associations:
    const typename ContainersConfiguration::vector_type &   predictors  ( ) const {return _predictors;}
	typename ContainersConfiguration::vector_type &   predictors() { return _predictors; }
    const Response& response    ( ) const {return _response;}

//  Commands:
    void setPredictorValue      ( uint32_t predictor, double value );
    void setResponseValue       ( uint32_t response,  double value );
    void calculateResidual      ( const PredictedValueFunctor& predicted );
    void setPredictedValue      ( double value );
    void setResponseNormalizer  ( double value );
	void setResponse			( double value ) { _response = value; }

	//multiply will multiple this basic_observation by the multiplier (both predictors and response, but not the normalizer or residual)
	//The normalizer may need to be modified, and residuals may need to be recalculated.  Both are the resonsibility of the caller of multiply.
	void multiply				( double multiplier );

//  Queries:
    virtual bool        isValid() const;
    double      responseValue       ( ) const {return _response();}
    double      predictedValue      ( ) const {return _predicted;}
    uint32_t    predictorCount      ( ) const {return numeric_cast<uint32_t>(_predictors.size());}
    double      residualValue       ( ) const {return _residual;}   
    double      responseNormalizer  ( ) const {return _response_normalizer;}

private:
    typename ContainersConfiguration::vector_type      _predictors;
    Response    _response;
    double      _predicted;
    double      _residual;
    double      _response_normalizer;
};

typedef basic_observation<DefaultContainers> Observation;
typedef boost::shared_ptr<Observation> ObservationPtr;

////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
basic_observation<ContainersConfiguration>::setPredictorValue( uint32_t predictor, double value )
{
    _predictors(predictor) = value;
}
////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
 basic_observation<ContainersConfiguration>::setResponseValue( uint32_t response, double value )
{
    _response = Response(value,response);
}
////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
 basic_observation<ContainersConfiguration>::setResponseNormalizer(double value )
{
    _response_normalizer = value;
}
////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
 basic_observation<ContainersConfiguration>::multiply( double multiplier )
{
    _response *= multiplier;
	std::for_each(_predictors.begin(), _predictors.end(), [&multiplier](double & val)
	{
		val *= multiplier;
	});
}

////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#include "Observation.inl"
#endif // MR_STATISTICAL_ANALYSIS_OLS_OBSERVATION_H
////////////////////////////////////////////////////////////////////////////////
