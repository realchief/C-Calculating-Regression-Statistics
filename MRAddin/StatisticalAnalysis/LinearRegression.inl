#include "StatisticalAnalysis/Observation.h"
#include "StatisticalAnalysis/RegressionMask.h"
#include "StatisticalAnalysis/LinearRegressionCalculator.h"
#include "DesignByContract.h"
#include "DebugNew.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template<typename ContainersConfiguration>
inline basic_linear_regression<ContainersConfiguration>::basic_linear_regression( typename basic_linear_regression<ContainersConfiguration>::AccumulatorPtr accumulator, MatrixInversionPolicyPtr matrix_inversion_policy )
:   _accumulator( accumulator ), _matrix_inversion_policy(matrix_inversion_policy)
{
	if (isNull(_matrix_inversion_policy))
	{
		_matrix_inversion_policy.reset(new DefaultMatrixInversionPolicy());
	}
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline int64_t
basic_linear_regression<ContainersConfiguration>::observationCount() const                       
{
    return _accumulator->observationCount();
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline int64_t
basic_linear_regression<ContainersConfiguration>::discardedCount() const                       
{
    return _accumulator->discardedCount();
}
///////////////////////////////////////////////////////////////////////////////

template<typename ContainersConfiguration>
inline RegressionResultPtr 
basic_linear_regression<ContainersConfiguration>::calculateResult( const RegressionMask& mask, const Matrix * merge_matrix, bool has_constant ) const
{
    basic_linear_regression_calculator<ContainersConfiguration> calculator( _accumulator->accumulations(), _accumulator->discardedCount() );

    return calculator.result( mask, *_matrix_inversion_policy, merge_matrix, has_constant );
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline RegressionResultPtr 
basic_linear_regression<ContainersConfiguration>::calculateResult( const Matrix * merge_matrix, bool has_constant ) const
{
    RegressionMask mask( _accumulator->predictorCount(), true );
    return calculateResult( mask, merge_matrix, has_constant);
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void
basic_linear_regression<ContainersConfiguration>::setEMALength( double ema_length )
{
    _accumulator->setEMALength( ema_length );                   
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void 
basic_linear_regression<ContainersConfiguration>::setAutoEMA(bool auto_ema)
{
    _accumulator->setAutoEMA( auto_ema );                       
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void
basic_linear_regression<ContainersConfiguration>::setEMABoundary()
{
    _accumulator->setEMABoundary();
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////