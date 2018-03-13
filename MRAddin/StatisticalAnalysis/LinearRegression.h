#ifndef MR_LINEAR_REGRESSION_H
#define MR_LINEAR_REGRESSION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/ObservationAccumulator.h"
#include "StatisticalAnalysis/DefaultMatrixInversionPolicy.h"

////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_linear_regression
{
public:   
	typedef boost::shared_ptr<basic_observation_accumulator<ContainersConfiguration> > AccumulatorPtr;
//  Lifetime
	explicit basic_linear_regression   (AccumulatorPtr accumulator, MatrixInversionPolicyPtr matrix_inversion_policy = MatrixInversionPolicyPtr());
            ~basic_linear_regression   ( ){};

//  Navigable Associations:
    AccumulatorPtr  accumulator() const {return _accumulator;}

//  Commands:
    void addObservation     ( const basic_observation<ContainersConfiguration> & observation );
    void addObservation     ( const basic_episode<ContainersConfiguration> & observation );
    void addObservation     ( double response, const Values& predictors );
    void setAutoEMA         ( bool auto_ema );
    void setEMALength       ( double ema_length );
    void setPredictorCount  ( size_t predictor_count );
    void setEMABoundary     ( );
    void reset              ( );

//  Queries:
    int64_t observationCount() const;
    int64_t discardedCount  () const;

    RegressionResultPtr calculateResult ( const RegressionMask& mask, const Matrix * merge_matrix = nullptr, bool has_constant = true  )  const;
    RegressionResultPtr calculateResult ( const Matrix * merge_matrix = nullptr, bool has_constant = true )                               const;

private:

//  Associations
    AccumulatorPtr  _accumulator;

	MatrixInversionPolicyPtr _matrix_inversion_policy;

//  Not Implemented:
    basic_linear_regression( );
};

typedef basic_linear_regression<DefaultContainers> LinearRegression;

///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void
basic_linear_regression<ContainersConfiguration>::addObservation( const basic_observation<ContainersConfiguration> & observation )
{
    _accumulator->add(observation);                       
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void
basic_linear_regression<ContainersConfiguration>::addObservation( double response, 
                                  const Values& predictors )
{
    Observation observation( predictors, response );
    _accumulator->add( observation );
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void
basic_linear_regression<ContainersConfiguration>::addObservation( const basic_episode<ContainersConfiguration> & observation )
{
    _accumulator->add(observation);                       
}
///////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
inline void
basic_linear_regression<ContainersConfiguration>::reset( )
{
    _accumulator->reset();                       
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////

#include "LinearRegression.inl"

#endif // MR_LINEAR_REGRESSION_H
////////////////////////////////////////////////////////////////////////////////