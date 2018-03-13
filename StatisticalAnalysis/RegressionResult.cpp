#include "Common.h"
#include "StatisticalAnalysis/RegressionResult.h"
#include "Infrastructure/MathUtils.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
        namespace
        {
            using infrastructure::math::NAN_NA;
        }
///////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
RegressionResult::RegressionResult  ( const RegressionMask& mask,
                                      uint64_t observation_count,
                                      uint64_t discarded_count,
                                      double sum_weights,
									  bool has_constant)
:   _mask                           ( mask ),
    _observation_count              ( observation_count ),
    _discarded_count                ( discarded_count ),
    _sum_weights                    ( sum_weights ),
    _inversion_failed               ( false ),
    _akaike_information_criteria    ( NAN_NA ),
    _beta                           ( mask.size(), NAN_NA ),
    _beta_covariance                ( mask.size(), mask.size() ),
    _beta_standard_error            ( mask.size(), NAN_NA ),
    _chi_squared                    ( NAN_NA ),
    _degrees_of_freedom             ( NAN_NA ),
    _residual_variance              ( NAN_NA ),
    _r_squared                      ( NAN_NA ),
    _r_squared_adjusted             ( NAN_NA ),
    _schwarz_information_criteria   ( NAN_NA ),
    _t_beta                         ( mask.size() ),
    _predictor_means                ( mask.size(), NAN_NA ),
    _predictor_std_devs             ( mask.size(), NAN_NA ),
    _response_mean                  ( NAN_NA ),
    _response_std_dev               ( NAN_NA ),
    _response_variance              ( NAN_NA ),
	_r_squared_null_model_has_model_cov_mat(NAN_NA),
	_r_squared_tested_model_returns_means(NAN_NA),
	_cross_validated_r2 (NAN_NA),
	_cross_validated_mse(NAN_NA),
	_cross_validated_log_likelihood(NAN_NA),
	_cross_validated_log_likelihood_null_model_has_cov_mat(NAN_NA),
	_cross_validated_log_likelihood_tested_model_returns_means(NAN_NA),
	_in_sample_log_likelihood(NAN_NA),
	_in_sample_log_likelihood_null_model_has_cov_mat(NAN_NA),
	_in_sample_log_likelihood_tested_model_returns_means(NAN_NA),
	_coefficients					( mask.size(), NAN_NA ),
	_coefficients_ols				( mask.size(), NAN_NA ),
	_sols_alpha						( mask.size(), NAN_NA ),
	_sols_space_base_change_matrix	(),//intentionally empty, set only in case of SOLS model building, client code should check if this matrix is empty or not
	_named_statistics(),
	_switched_predictor_names( mask.size() ),
	_prune_group_id( mask.size(), 0 ),
	_has_constant  ( has_constant )
{    
    _beta_covariance.clear();
	uint32_t counter = 0;
	std::generate(_prune_group_id.begin(), _prune_group_id.end(), [&counter]()->uint32_t { return counter++; });
}

RegressionResult::RegressionResult() :
_mask(),
_observation_count(0),
_discarded_count(0),
_sum_weights(NAN_NA),
_inversion_failed(),
_akaike_information_criteria(NAN_NA),
_beta(),
_beta_covariance(),
_beta_standard_error(),
_chi_squared(NAN_NA),
_degrees_of_freedom(NAN_NA),
_f_ratio(NAN_NA),
_residual_variance(NAN_NA),
_r_squared(NAN_NA),
_r_squared_adjusted(NAN_NA),
_schwarz_information_criteria(NAN_NA),
_t_beta(),
_predictor_means(),
_predictor_std_devs(),
_response_mean(NAN_NA),
_response_std_dev(NAN_NA),
_response_variance(NAN_NA),
_r_squared_null_model_has_model_cov_mat(NAN_NA),
_r_squared_tested_model_returns_means(NAN_NA),
_cross_validated_r2 (NAN_NA),
_cross_validated_mse(NAN_NA),
_cross_validated_log_likelihood(NAN_NA),
_cross_validated_log_likelihood_null_model_has_cov_mat(NAN_NA),
_cross_validated_log_likelihood_tested_model_returns_means(NAN_NA),
_in_sample_log_likelihood(NAN_NA),
_in_sample_log_likelihood_null_model_has_cov_mat(NAN_NA),
_in_sample_log_likelihood_tested_model_returns_means(NAN_NA),
_coefficients(),
_coefficients_ols(),
_sols_alpha(),
_sols_space_base_change_matrix(),
_named_statistics(),
_switched_predictor_names(),
_prune_group_id(),
_has_constant( true ) //set true by default so as to use historical r-squared calculation (with constant) by default
{
}

void 
RegressionResult::setBeta( const RawResults& values )
{
	_beta = values;
	const size_t num_predictors = values.size();
	_coefficients_ols.resize(num_predictors, NAN_NOT_SET);

	size_t old_size = _prune_group_id.size();
	uint32_t id = 0;
	std::vector<uint32_t>::const_iterator I = std::max_element(_prune_group_id.cbegin(), _prune_group_id.cend());
	if (I != _prune_group_id.cend())	id = *I;
	_prune_group_id.resize(num_predictors);
	for (size_t i = old_size; i<num_predictors; ++i)
	{
		_prune_group_id[i] = (++id);
	}
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   //  namespace mr
///////////////////////////////////////////////////////////////////////////////
