#include "StatisticalAnalysis/Observation.h"
#include "StatisticalAnalysis/Response.h"
#include "StatisticalAnalysis/SwitchedContainersConfig.h"
#include "DesignByContract.h"
#include "DebugNew.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
///
template <typename ContainersConfiguration>
inline basic_std_accumulation<ContainersConfiguration>::basic_std_accumulation(  )
{
}
///////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
template <typename ContainersConfiguration>
inline basic_std_accumulation<ContainersConfiguration>::basic_std_accumulation( uint32_t predictor_count )
:   basic_accumulation<ContainersConfiguration>( predictor_count )
{
}
///////////////////////////////////////////////////////////////////////////////

///
/// Add an observation to the accumulation
///

template <typename ContainersConfiguration>
inline void 
basic_std_accumulation<ContainersConfiguration>::accumulate( const basic_observation<ContainersConfiguration> & observation )
{
    const Response& response = observation.response();
	const DefaultContainers::vector_type & predictors = observation.predictors();

    _sum_x  +=  predictors;
    _sum_y  +=  response();

    _sum_yy += (response() * response());
    
	_sum_xy += (predictors * response());
	
	for (size_t i = 0, imax = predictors.size(); i < imax; ++i)
	{
		const double predictor_value_i = predictors(i);
		for (size_t j = i; j < imax; ++j)
		{
			_sum_xx(i, j) += predictor_value_i * predictors(j);
		}
	}

    ++_sum_weights;
    ++_observations;
}

template <>
inline void 
basic_std_accumulation<switched_models_common::SparseContainers>::accumulate( const basic_observation<switched_models_common::SparseContainers> & observation )
{
    const Response& response = observation.response();
	const switched_models_common::SparseContainers::vector_type & predictors = observation.predictors();
	const double response_value = response();
    
	_sum_y  +=  response_value;
    _sum_yy += (response_value * response_value);
    
	for (size_t i = 0, imax = predictors.filled(); i < imax; ++i)
	{
		const size_t row = predictors.index_data()[i];

		const double predictor_value_i = predictors.value_data()[i];
		_sum_x(row) += predictor_value_i;
		_sum_xy(row) += response_value * predictor_value_i;

		for (size_t j = i; j < imax; ++j)
		{
			const size_t col = predictors.index_data()[j];
			_sum_xx(row, col) += predictor_value_i * predictors.value_data()[j];
		}
	}

    ++_sum_weights;
    ++_observations;
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
