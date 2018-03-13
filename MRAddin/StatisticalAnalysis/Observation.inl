#include "StatisticalAnalysis/SwitchedContainersConfig.h"
#include <boost/detail/algorithm.hpp>

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure::math;
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
inline bool isInvalid( double value )
{
    return ( _isnan(value) || !_finite(value) );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template <typename ContainersConfiguration>
inline basic_observation<ContainersConfiguration>::basic_observation( Values::size_type predictor_count )
:   _predictors ( ZeroVector(predictor_count) ),
    _response   ( NAN_NOT_SET ),
    _predicted  ( NAN_NOT_SET ),
    _residual   ( NAN_NA ),
	_response_normalizer ( NAN_NOT_SET )
{
}

template <>
inline basic_observation<switched_models_common::SparseContainers>::basic_observation( Values::size_type predictor_count )
:   _predictors ( predictor_count ),
	_response   ( NAN_NOT_SET ),
	_predicted  ( NAN_NOT_SET ),
	_residual   ( NAN_NA ),
	_response_normalizer ( NAN_NOT_SET )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template <typename ContainersConfiguration>
inline basic_observation<ContainersConfiguration>::basic_observation( const typename ContainersConfiguration::vector_type & predictors,
                          const Response& response )
:   _predictors ( predictors ),
    _response   ( response ),
    _predicted  ( NAN_NOT_SET ),
    _residual   ( NAN_NA ),
	_response_normalizer ( NAN_NOT_SET )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template <typename ContainersConfiguration>
inline basic_observation<ContainersConfiguration>::basic_observation( const typename ContainersConfiguration::vector_type & predictors,
                          double response )
:   _predictors ( predictors ),
    _response   ( response ),
    _predicted  ( NAN_NOT_SET ),
    _residual   ( NAN_NA ),
	_response_normalizer ( NAN_NOT_SET )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template <typename ContainersConfiguration>
inline basic_observation<ContainersConfiguration>::basic_observation( const Values& predictors,
                          double response )
:   _predictors ( predictors.size() ),
    _response   ( response ),
    _predicted  ( NAN_NOT_SET ),
    _residual   ( NAN_NA ),
	_response_normalizer ( NAN_NOT_SET )
{
    std::copy( predictors.begin(), predictors.end(), _predictors.begin() );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return true if response and all predictor values are valid
///
template <typename ContainersConfiguration>
inline bool
basic_observation<ContainersConfiguration>::isValid() const
{
    return !isInvalid( _response() ) 
		&& !boost::any_if(_predictors.begin(), _predictors.end(), isInvalid) ;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Use the predicted value functor to calulate the predicted value 
/// Set the predicted and residual values
///
template <typename ContainersConfiguration>
inline void 
basic_observation<ContainersConfiguration>::calculateResidual( const PredictedValueFunctor& predicted )
{
    DBC_REQUIRE( isValid() );

    setPredictedValue( predicted( _predictors ) );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Set the predicted and residual values
///
template <typename ContainersConfiguration>
inline void
basic_observation<ContainersConfiguration>::setPredictedValue( double predicted )
{
    _predicted = predicted;

    _residual = isNaN( _predicted )
        ?   NAN_NA
        :   _response() - _predicted;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
