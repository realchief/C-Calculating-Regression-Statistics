#include "StatisticalAnalysis/RegressionResult.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/GaussJordanInversion.h"
#include "StatisticalAnalysis/AccumulationGroup.h"
#include "StatisticalAnalysis/MatrixInversionCanceled.h"
#include "Infrastructure/MathUtils.h"
#include "StatisticalAnalysis/FtSignificance.h"
#include "DesignByContract.h"
#include "DebugNew.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace infrastructure::math;
///////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
template <typename ContainersConfiguration>
inline basic_linear_regression_calculator<ContainersConfiguration>::basic_linear_regression_calculator
    ( const basic_accumulation_group<ContainersConfiguration> & accumulations, uint64_t discarded )
:   _accumulations  ( accumulations )
,   _discarded      ( discarded )
{
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate the linear regression result based on the active predictors
///
template <typename ContainersConfiguration>
inline RegressionResultPtr 
basic_linear_regression_calculator<ContainersConfiguration>::result( const RegressionMask& mask, MatrixInversionPolicy & inversion_policy, const Matrix * merge_matrix, bool has_constant) const
{
	if (merge_matrix && !mask.usesAllPredictors())
	{
		BOOST_THROW_EXCEPTION(std::runtime_error("Switch pruning with regression mask not using all predictors is currently unsupported."));
	}
    DBC_REQUIRE( mask.size() == _accumulations.predictorCount() );

    const basic_accumulation<ContainersConfiguration> & accumulation 
        = _accumulations[mask.responseIndex()];

    if( accumulation.observations() <= _accumulations.predictorCount())
    {
		RegressionResultPtr result( new RegressionResult( 
			mask, accumulation.observations(), discarded(), accumulation.sumWeights(), has_constant ) );

		return result;
    }

    RegressionResultPtr result( new RegressionResult( 
        mask, accumulation.observations(), discarded(), accumulation.sumWeights(), has_constant ) );

    result->setResponseMean     ( accumulation.responseMean() );
    result->setResponseStdDev   ( accumulation.responseStdDev() );
    result->setResponseVariance ( responseVariance  (accumulation) );
    result->setPredictorMeans   ( predictorMeans    (accumulation) );
    result->setPredictorStdDevs ( predictorStdDevs  (accumulation) );
    result->setDoF              ( DoF( *result ) );

	size_t num_merged_columns = 0;
	if (merge_matrix)
	{
		DBC_REQUIRE( merge_matrix->size1() == _accumulations.predictorCount() );
		num_merged_columns = merge_matrix->size2();
	}

    try
    {
		const SymMatrix & xtx = sumXX(accumulation, *result, merge_matrix);
        const Matrix & inv_xx = inversion_policy.invert(xtx);

		result->setBeta             ( calculateBetas( inv_xx, accumulation, *result, merge_matrix ) );
		result->setCoefficients     ( result->beta() );
		if (merge_matrix)
		{
			result->setCoefficientsOLS  ( calculateBetas( inversion_policy.invert(sumXX(accumulation, *result, nullptr)), accumulation, *result, nullptr ) );
		}
		else
		{
			result->setCoefficientsOLS  ( result->beta() );
		}
        result->setChiSquared       ( chiSquared(accumulation, *result) );
        result->setResidualVariance ( residualVariance(*result) );
        result->setRSquared         ( RSquared(*result) );
        result->setRSquaredAdjusted ( adjustedRSquared(*result) );
        result->setSchwarz          ( schwarz(*result) );
        result->setBetaCovariance   ( betaCovariance( inv_xx, accumulation, *result, merge_matrix ) );
		//result->setXtX              ( xtx );
        result->setBetaStdErr       ( betaStdErr( *result ) );
        result->setTBeta            ( betaT     ( *result ) );
        result->setFRatio           ( FRatio(*result) );
        result->setAkaike           ( akaike(*result) );
		result->setInversionFailed(inversion_policy.colinearityDetected());
    }
    catch( GaussJordanInversionFailure& )
    {
        result->setInversionFailed(true);
    }
	catch ( MatrixInversionCanceled &)
	{
		result->setInversionFailed(true);
	}
    catch( ... )
    {
        result->setInversionFailed(true);
    }

    return result;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the y-variance
///
template <typename ContainersConfiguration>
inline double
basic_linear_regression_calculator<ContainersConfiguration>::responseVariance( const basic_accumulation<ContainersConfiguration> & accumulation ) const
{
    const double n              = accumulation.sumWeights();
    const double sum_of_squares = accumulation.sumYY();
    const double sum            = accumulation.sumY();

    return n > 1.0 
        ?   ( n*sum_of_squares - sum*sum ) 
                 /  ( n * ( n - 1.0 ) )
        : NAN_NA;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the means of each predictor
///
template <typename ContainersConfiguration>
inline RawResults
basic_linear_regression_calculator<ContainersConfiguration>::predictorMeans( const basic_accumulation<ContainersConfiguration> & accumulation  ) const
{
    const uint32_t predictors = accumulation.predictors();

    RawResults predictor_means( predictors, NAN_NA );

    for(uint32_t i = 0; i < predictors; ++i)
    {
        predictor_means[i] = accumulation.predictorMean(i);
    }

    return predictor_means;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the standard deviation of each predictor
///
template <typename ContainersConfiguration>
inline RawResults
basic_linear_regression_calculator<ContainersConfiguration>::predictorStdDevs( const basic_accumulation<ContainersConfiguration> & accumulation ) const
{
    const uint32_t predictors = accumulation.predictors();

    RawResults predictor_std_devs( predictors, NAN_NA );

    for(uint32_t i = 0; i < predictors; ++i)
    {
        predictor_std_devs[i] = accumulation.predictorStdDev(i);
    }

    return predictor_std_devs;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the degrees-of-freedom
///
template <typename ContainersConfiguration>
inline double 
basic_linear_regression_calculator<ContainersConfiguration>::DoF( const RegressionResult& interim_result ) const
{
    return interim_result.sumWeights() - interim_result.mask().size();
}
///////////////////////////////////////////////////////////////////////////////

///
/// transfer the accumulation SumXX to the active predictor matrix
///
template <typename ContainersConfiguration>
inline SymMatrix
basic_linear_regression_calculator<ContainersConfiguration>::sumXX( const basic_accumulation<ContainersConfiguration> & accumulation,
                                   const RegressionResult& interim_result,
								   const Matrix * merge_matrix) const
{
	namespace ublas = boost::numeric::ublas;

	if (merge_matrix && !interim_result.mask().usesAllPredictors())
	{
		BOOST_THROW_EXCEPTION(std::runtime_error("Switch pruning with regression mask not using all predictors is currently unsupported."));
	}

    if( interim_result.mask().usesAllPredictors() )
    {
		if (merge_matrix)
		{
			return SymMatrix(static_cast<ublas::matrix<double> >(ublas::prod(static_cast<ublas::matrix<double> >(ublas::prod(ublas::trans(*merge_matrix), accumulation.sumXX())), *merge_matrix)));
		}
		else
		{
			return accumulation.sumXX();
		}
    }

    const PredictorIndices& indices = interim_result.mask().indices( );

	const SymMatrix::size_type size = indices.size();

	SymMatrix sum_xx( size );

	const SymMatrix & sum_XX = accumulation.sumXX();
   
    for(size_t p = 0, count = size; p < count; ++p)
    {
        for(size_t q = p; q < count; ++q)
        {
            sum_xx(p,q) = sum_XX(indices[p], indices[q]);
        }
    }

	return sum_xx;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Build the vector of Beta values
//      beta = _sum_xx_inverse(num_x, num_x) . _sum_xy(num_x, 1)
///
template <typename ContainersConfiguration>
inline RawResults
basic_linear_regression_calculator<ContainersConfiguration>::calculateBetas( const Matrix & inverse_sum_XX,
                                            const basic_accumulation<ContainersConfiguration> & accumulation,
                                            const RegressionResult& interim_result,
											const Matrix * merge_matrix) const
{
	RawResults beta;
	if (!merge_matrix)
	{
		const PredictorMask&    active  = interim_result.mask()();
		const PredictorIndices& indices = interim_result.mask().indices();                                           
		const uint32_t          size    = interim_result.mask().size();

		beta.resize( size, 0.0 );
	
		for(uint32_t p = 0, i = 0; p < size; ++p )
		{
			if( active[p] )
			{
				for(uint32_t q = 0; q < indices.size(); ++q)
				{
					beta[p] += inverse_sum_XX(i,q) 
							* accumulation.sumXY()[indices[q]];
				}
				++i;
			}
		}
	}
	else
	{
		if (!interim_result.mask().usesAllPredictors())
		{
			BOOST_THROW_EXCEPTION(std::runtime_error("SwitchPruning merge matrix with OLS currently unsupported unless mask uses all predictors."));
		}

		namespace ublas = boost::numeric::ublas;
		const size_t original_order = merge_matrix->size1();
		const size_t new_order      = merge_matrix->size2();

		DBC_REQUIRE(new_order == inverse_sum_XX.size1() && new_order == inverse_sum_XX.size2());
		beta.resize( original_order );
		const Matrix invXXM( ublas::prod( inverse_sum_XX, ublas::trans(*merge_matrix) ) );
		const Vector & xy( accumulation.sumXY() );
		const Vector beta_merge( ublas::prod( invXXM, xy ) );
		const Vector ubeta( ublas::prod( *merge_matrix, beta_merge ) );
		DBC_REQUIRE( ubeta.size() == original_order);

		std::copy( ubeta.begin(), ubeta.end(), beta.begin() );
	}
	
    return beta;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return chi-squared
///     _sum_yy - 2 _sum_xy' . beta + beta' . _sum_xx . beta'
///
template <typename ContainersConfiguration>
inline double 
basic_linear_regression_calculator<ContainersConfiguration>::chiSquared( const basic_accumulation<ContainersConfiguration> & accumulation,
                                        const RegressionResult& interim_result ) const
{
    const PredictorIndices& indices = interim_result.mask().indices();                                           

	const SymMatrix & sum_XX = accumulation.sumXX();

    double chi_squared = accumulation.sumYY();

    for( size_t p = 0, count = indices.size(); p < count; ++p )
    {
        size_t i = indices[p];

        double xx_beta_i = 0;

        for( size_t q = 0; q < count; ++q )
        {
            size_t j = indices[q];

            xx_beta_i += sum_XX(i,j) * interim_result.beta()[j];
        }

        chi_squared += interim_result.beta()[i] * (xx_beta_i - 2.0 * accumulation.sumXY()[i]);
    }

    return max( chi_squared, 0.0 );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the residual variance
///
template <typename ContainersConfiguration>
inline double
basic_linear_regression_calculator<ContainersConfiguration>::residualVariance( const RegressionResult& interim_result ) const
{
    return interim_result.chiSquared() / interim_result.DoF();
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate the beta-covariance
///     _sum_xx_inverse chi_squared / (_num_observations - _num_x)
///
template <typename ContainersConfiguration>
inline SymMatrix
basic_linear_regression_calculator<ContainersConfiguration>::betaCovariance( const Matrix & inverse_sum_XX,
                                            const basic_accumulation<ContainersConfiguration> & accumulation,
                                            const RegressionResult& interim_result,
											const Matrix * merge_matrix ) const
{
	if (merge_matrix && !interim_result.mask().usesAllPredictors())
	{
		BOOST_THROW_EXCEPTION(std::runtime_error("Switch pruning with regression mask not using all predictors is currently unsupported."));
	}

    const double res_var = interim_result.residualVariance();
	SymMatrix beta_covariance;

	if (!merge_matrix)
	{
		const PredictorIndices& indices = interim_result.mask().indices();
		const boost::uint32_t size = accumulation.predictors();

		beta_covariance = ZeroMatrix( size, size );

		for(size_t p = 0, count = indices.size(); p < count; ++p)
		{
			const size_t& i = indices[p];

			for(size_t q = p; q < count; ++q )
			{
				const size_t& j = indices[q];

				beta_covariance(i,j) = inverse_sum_XX(p,q) * res_var;
			}
		}
	}
	else
	{
		namespace ublas = boost::numeric::ublas;
		beta_covariance = res_var * ublas::prod( *merge_matrix, static_cast<Matrix>(ublas::prod( inverse_sum_XX, ublas::trans(*merge_matrix) ) ) );
	}

    return beta_covariance;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the beta std errors
///
template <typename ContainersConfiguration>
inline RawResults
basic_linear_regression_calculator<ContainersConfiguration>::betaStdErr( const RegressionResult& interim_result ) const
{
    const PredictorMask&    active  = interim_result.mask()();
    const uint32_t          size    = interim_result.mask().size();

    RawResults beta_std_err( size, 0.0 );

    for(size_t i = 0; i < size; ++i)
    {
        if( active[i] )
        {
            beta_std_err[i] = sqrt(interim_result.betaCovariance()(i,i));
        }
    }
    return beta_std_err;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the beta t statistics
///
template <typename ContainersConfiguration>
inline StatisticsResults
basic_linear_regression_calculator<ContainersConfiguration>::betaT( const RegressionResult& interim_result ) const
{
    const PredictorMask&    active  = interim_result.mask()();
    const uint32_t          size    = interim_result.mask().size();

    StatisticsResults t_beta_results( size );

    for(size_t i = 0; i < size; ++i)
    {
        if( active[i] )
        {
            const double t_beta = interim_result.beta()[i] 
                                / interim_result.betaStdErr()[i];

            t_beta_results[i]   = StatisticsResult( 
                t_beta, TSig( t_beta, interim_result.DoF() ));
        }
    }
    return t_beta_results;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the r-squared
///
template <typename ContainersConfiguration>
inline double
basic_linear_regression_calculator<ContainersConfiguration>::RSquared( const RegressionResult& interim_result ) const
{
    const double n      = interim_result.sumWeights();
    const double chi_sq = interim_result.chiSquared();
    const double y_var  = interim_result.responseVariance();
	const double y_mean = interim_result.responseMean();

	const double sst = interim_result.hasConstant() 
		? ((n - 1.0) * y_var )                                     //sum of squared deviations from mean
		: ((n - 1.0) * y_var + n * (y_mean * y_mean));             //sum of squared responses

    return n > 1.0
        ?   1.0 - chi_sq / sst 
        :   NAN_NA;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the adjusted r-squared
///
template <typename ContainersConfiguration>
inline double
basic_linear_regression_calculator<ContainersConfiguration>::adjustedRSquared( const RegressionResult& interim_result ) const
{
    const double n      = interim_result.sumWeights();
    const double d_o_f  = interim_result.DoF();
    const double r_sq   = interim_result.rSquared();

    return n > 1.0
        ?  1.0 - (1.0 - r_sq) * (n - 1.0) / d_o_f
        :  NAN_NA;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the F-ratio
///
template <typename ContainersConfiguration>
inline StatisticsResult
basic_linear_regression_calculator<ContainersConfiguration>::FRatio( const RegressionResult& interim_result ) const
{
    const double d_o_f  = interim_result.DoF();
    const double r_sq   = interim_result.rSquared();
    const double m      = interim_result.sumWeights() - interim_result.DoF();

    const double f_ratio =       d_o_f * r_sq 
                         / ((m - 1.0) * (1.0 - r_sq));

    return StatisticsResult( f_ratio, FSig( f_ratio, d_o_f, m-1.0 ) );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the akaike information criteria
///
template <typename ContainersConfiguration>
inline double
basic_linear_regression_calculator<ContainersConfiguration>::akaike( const RegressionResult& interim_result ) const
{
    const double n      = interim_result.sumWeights();
    const double r_sq   = interim_result.rSquared();
    const double m      = n - interim_result.DoF();

    return (1.0 - r_sq) + 2.0 * (m - 1.0) / (n - 1.0);
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the schwarz information criteria
///
template <typename ContainersConfiguration>
inline double
basic_linear_regression_calculator<ContainersConfiguration>::schwarz( const RegressionResult& interim_result ) const
{
    const double n      = interim_result.sumWeights();
    const double r_sq   = interim_result.rSquared();
    const double m      = n - interim_result.DoF();

    return  (1.0 - r_sq) + log(n) * (m - 1.0) / (n - 1.0);
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
