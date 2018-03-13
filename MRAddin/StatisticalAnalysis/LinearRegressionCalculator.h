#ifndef MR_REGRESSION_RESULT_CALCULATOR_H
#define MR_REGRESSION_RESULT_CALCULATOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/Accumulation.h"
#include "StatisticalAnalysis/AccumulationGroup.h"
#include "NumericContainersConfiguration.h"
#include "StatisticalAnalysis/SwitchedContainersConfig.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
        namespace
        {
            using  std::vector;
        }
///////////////////////////////////////////////////////////////////////////////
class ObservationAccumulation;
class RegressionResult;
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_linear_regression_calculator 
{
public:
    explicit basic_linear_regression_calculator ( const basic_accumulation_group<ContainersConfiguration> & accumulations,
                                                  uint64_t discarded );

//  Queries:
    RegressionResultPtr result  ( const RegressionMask& mask, MatrixInversionPolicy & inversion_policy, const Matrix * merge_matrix = nullptr, bool has_constant = true )  const;

private:
	SymMatrix			sumXX			( const basic_accumulation<ContainersConfiguration> & accumulation,
                                          const RegressionResult& interim_result,
										  const Matrix * merge_matrix = nullptr )  const;

    double              DoF             ( const RegressionResult& interim_result )  const;

    double              chiSquared      ( const basic_accumulation<ContainersConfiguration> & accumulation,
                                          const RegressionResult& interim_result )  const;

    double              responseVariance( const basic_accumulation<ContainersConfiguration> & accumulation )        const;

    double              residualVariance( const RegressionResult& interim_result )  const;

    double              RSquared        ( const RegressionResult& interim_result )  const;

    double              adjustedRSquared( const RegressionResult& interim_result )  const;

    double              akaike          ( const RegressionResult& interim_result )  const;

    double              schwarz         ( const RegressionResult& interim_result )  const;

    StatisticsResult    FRatio          ( const RegressionResult& interim_result )  const;

	RawResults          calculateBetas  ( const Matrix & sum_XX_inverse,
                                          const basic_accumulation<ContainersConfiguration> & accumulation,
                                          const RegressionResult& interim_result,
										  const Matrix * merge_matrix = nullptr )  const;

	SymMatrix			betaCovariance  ( const Matrix & inverse_sum_XX,
										  const basic_accumulation<ContainersConfiguration> & accumulation,
										  const RegressionResult& interim_result,
										  const Matrix * merge_matrix)  const;

    RawResults          betaStdErr      ( const RegressionResult& interim_result )  const;

    StatisticsResults   betaT           ( const RegressionResult& interim_result )  const;

    RawResults          predictorMeans  ( const basic_accumulation<ContainersConfiguration> & accumulation )        const;

    RawResults          predictorStdDevs( const basic_accumulation<ContainersConfiguration> & accumulation )        const;         

    uint64_t            discarded       ( ) const {return _discarded;}

    const basic_accumulation_group<ContainersConfiguration> &  _accumulations;

private:

    uint64_t _discarded;

    basic_linear_regression_calculator  ( );
    basic_linear_regression_calculator  ( const basic_linear_regression_calculator& );
    void operator=                      ( const basic_linear_regression_calculator& );
};

typedef basic_linear_regression_calculator<DefaultContainers> LinearRegressionCalculator;

///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr

#include "LinearRegressionCalculator.inl"

///////////////////////////////////////////////////////////////////////////////
#endif // MR_REGRESSION_RESULT_CALCULATOR_H
///////////////////////////////////////////////////////////////////////////////
