#include "Common.h"
#include "StatisticalAnalysis/R2Calculator.h"
#include "Infrastructure/MathUtils.h"
#include "StatisticalAnalysis/GaussJordanInversion.h"
#include "StatisticalAnalysis/CholeskyDecomposition.h"
#include "Infrastructure/SettingsManager.h"

using namespace boost::numeric::ublas;

namespace mr
{
    namespace statistical_analysis
    {

R2Calculator::R2Calculator(const std::vector<double> & response_means,
						   const std::vector<double> & response_variance) :
_covariance_matrix(),
_covariance_matrix_inverted(),
_means(response_means),
_variances(response_variance),
_cov_mat_log_determinant(),
_cov_mat_null_model_log_determinant(),
_null_model_cov_mat_diagonal(response_means.size()),
_universal_index_set(),
_chi_2(),
_model_cov_submatrix_log_determinant(),
_null_model_cov_submatrix_log_determinant(),
_cov_submatrix_inverted(),
_null_model_submatrix_diag_inverted(),
_old_nan_positions(),
_samples_count(),
_chi_2_null_model_has_model_cov_mat(),
_chi_2_true_model_response_is_mean()
{
	DBC_REQUIRE(response_variance.size() == _means.size());
	//to test that for tested model==null model we have 0 R^2
	/*for (size_t i = 0; i < covariances.size1(); ++i)
	{
	for (size_t j = i + 1; j < covariances.size1(); ++j)
	{
	_covariance_matrix(i, j) = 0;
	}
	}*/
	_cov_mat_null_model_log_determinant = 0.0;
	_cov_mat_log_determinant = 0.0;

	for (std::vector<double>::size_type i = 0; i < response_variance.size(); ++i)
	{
		_null_model_cov_mat_diagonal[i] = pow(response_variance[i], 2);
		if (response_variance[i] > 0.0)
		{
			_cov_mat_log_determinant += 2 * log(response_variance[i]);
		}
	}
}

R2Calculator::R2Calculator(const boost::numeric::ublas::symmetric_matrix<double> & covariances,
						   const std::vector<double> & response_means) :
_covariance_matrix(covariances),
_covariance_matrix_inverted(),
_means(response_means),
_cov_mat_log_determinant(),
_cov_mat_null_model_log_determinant(),
_null_model_cov_mat_diagonal(covariances.size1()),
_universal_index_set(),
_chi_2(),
_model_cov_submatrix_log_determinant(),
_null_model_cov_submatrix_log_determinant(),
_cov_submatrix_inverted(),
_null_model_submatrix_diag_inverted(),
_old_nan_positions(),
_samples_count(),
_chi_2_null_model_has_model_cov_mat(),
_chi_2_true_model_response_is_mean()
{
	DBC_REQUIRE(_covariance_matrix.size1() == _means.size());
	//to test that for tested model==null model we have 0 R^2
	/*for (size_t i = 0; i < covariances.size1(); ++i)
	{
		for (size_t j = i + 1; j < covariances.size1(); ++j)
		{
			_covariance_matrix(i, j) = 0;
		}
	}*/

	_cov_mat_log_determinant = choleskyDecompositionBasedLogDet(_covariance_matrix, _covariance_matrix_inverted);
	_cov_mat_null_model_log_determinant = 0.0;

	for (size_t i = 0; i < _covariance_matrix.size1(); ++i)
	{
		_null_model_cov_mat_diagonal[i] = _covariance_matrix(i, i);
		_universal_index_set.fast_insert(i);
		_cov_mat_null_model_log_determinant += log(_null_model_cov_mat_diagonal[i]);
	}
}

R2Calculator::R2Calculator(const boost::numeric::ublas::symmetric_matrix<double> & covariances,
						   const std::vector<double> & response_means,
						   const std::vector<double> & response_variance) :
_covariance_matrix(covariances),
_covariance_matrix_inverted(),
_means(response_means),
_variances(response_variance),
_cov_mat_log_determinant(),
_cov_mat_null_model_log_determinant(),
_null_model_cov_mat_diagonal(covariances.size1()),
_universal_index_set(),
_chi_2(),
_model_cov_submatrix_log_determinant(),
_null_model_cov_submatrix_log_determinant(),
_cov_submatrix_inverted(),
_null_model_submatrix_diag_inverted(),
_old_nan_positions(),
_samples_count(),
_chi_2_null_model_has_model_cov_mat(),
_chi_2_true_model_response_is_mean()
{
	DBC_REQUIRE(_covariance_matrix.size1() == _means.size());
	DBC_REQUIRE(_covariance_matrix.size1() == response_variance.size());
	//to test that for tested model==null model we have 0 R^2
	/*for (size_t i = 0; i < covariances.size1(); ++i)
	{
	for (size_t j = i + 1; j < covariances.size1(); ++j)
	{
	_covariance_matrix(i, j) = 0;
	}
	}*/

	_cov_mat_log_determinant = choleskyDecompositionBasedLogDet(_covariance_matrix, _covariance_matrix_inverted);
	_cov_mat_null_model_log_determinant = 0.0;

	for (size_t i = 0; i < _covariance_matrix.size1(); ++i)
	{
		_null_model_cov_mat_diagonal[i] = pow(response_variance[i], 2);
		_universal_index_set.fast_insert(i);
		_cov_mat_null_model_log_determinant += log(_null_model_cov_mat_diagonal[i]);
	}
}

/// Reference: https://ncalculators.com/statistics/r-squared-calculator.htm
/// Reference: https://en.wikipedia.org/wiki/Coefficient_of_determination

double
R2Calculator::choleskyDecompositionBasedLogDet(const boost::numeric::ublas::symmetric_matrix<double> & the_matrix,
											   boost::numeric::ublas::symmetric_matrix<double> & inv_matrix) const
{
	triangular_matrix<double, lower> L;
	choleskyDecomposition(the_matrix, L);
	triangular_matrix<double, upper> L_inv;
	choleskyInvert(L, L_inv);

	/*create inverse*/
	inv_matrix = prod(L_inv, trans(L_inv));

	double log_det = 0.0;
	for (size_t i = 0; i < L.size1(); ++i)
	{
		log_det += log(L(i, i));
	}
	//det(the_matrix) = (L(0, 0) * L(1, 1) * ... * L(matrix_size - 1, matrix_size - 1) )^2, that's why we have 2*log
	return 2 * log_det;
}


bool
R2Calculator::containsNaNs(boost::numeric::ublas::vector<double> & response_min_means) const
{
	return boost::any_if(response_min_means, &infrastructure::math::isNaN<double>);
}


symmetric_matrix<double>
R2Calculator::getReOrderedMatrix(const symmetric_matrix<double> & matrix_to_reorder, const infrastructure::lazy_set<size_t> & nan_positions) const
{
	infrastructure::lazy_set<size_t> non_nan_entries;
	std::set_difference(_universal_index_set.begin(), _universal_index_set.end(),
						nan_positions.begin(), nan_positions.end(),
						std::inserter(non_nan_entries, non_nan_entries.begin()));


	symmetric_matrix<double> P(non_nan_entries.size(), non_nan_entries.size());
	
	size_t p_matrix_row = 0;
	size_t p_matrix_column = 0;
	for (infrastructure::lazy_set<size_t>::const_iterator I = non_nan_entries.begin(), IEnd = non_nan_entries.end(); I != IEnd; ++I)
	{
        p_matrix_column = 0;
		for (infrastructure::lazy_set<size_t>::const_iterator J = non_nan_entries.begin(), JEnd = non_nan_entries.end(); J != JEnd; ++J)
		{
			P(p_matrix_row, p_matrix_column) = matrix_to_reorder(*I, *J);
			++p_matrix_column;
		}
		++p_matrix_row;
	}
	
	DBC_CHECK(p_matrix_column == P.size2());
	DBC_CHECK(p_matrix_row == P.size1());

	return P;
}

void
R2Calculator::calcDetAndCovarianceInverseOfModel(const infrastructure::lazy_set<size_t> & nan_entries_indices,
										double & log_determinant,
										boost::numeric::ublas::symmetric_matrix<double> & covariances_inverted) const
{
	const symmetric_matrix<double> reordered_cov_mat = getReOrderedMatrix(_covariance_matrix, nan_entries_indices);
	log_determinant = choleskyDecompositionBasedLogDet(reordered_cov_mat, covariances_inverted);
}

void
R2Calculator::calcDetAndCovarianceInverseOfDiagonalCovMat(const infrastructure::lazy_set<size_t> & nan_entries_indices,
														  const std::vector<double> & cov_mat_diagonal,
														  double & log_determinant,
														  boost::numeric::ublas::diagonal_matrix<double> & null_model_covariances_inverted) const
{
	log_determinant = 0;

	infrastructure::lazy_set<size_t> non_nan_entries;
	std::set_difference(_universal_index_set.begin(), _universal_index_set.end(),
						nan_entries_indices.begin(), nan_entries_indices.end(),
						std::inserter(non_nan_entries, non_nan_entries.begin()));

	null_model_covariances_inverted = boost::numeric::ublas::diagonal_matrix<double>(non_nan_entries.size());

	size_t ret_vec_idx = 0;
	for (infrastructure::lazy_set<size_t>::const_iterator I = non_nan_entries.begin(), IEnd = non_nan_entries.end(); I != IEnd; ++I)
	{
		log_determinant += log(cov_mat_diagonal[*I]);
		null_model_covariances_inverted(ret_vec_idx, ret_vec_idx) = 1.0 / cov_mat_diagonal[*I];
		++ret_vec_idx;
	}
}


void 
R2Calculator::calcDetAndCovarianceInverseOfNullModel(const infrastructure::lazy_set<size_t> & nan_entries_indices,
													 double & log_determinant,
													 boost::numeric::ublas::diagonal_matrix<double> & null_model_covariances_diag_inverted) const
{
	calcDetAndCovarianceInverseOfDiagonalCovMat(nan_entries_indices,
												_null_model_cov_mat_diagonal,
												log_determinant,
												null_model_covariances_diag_inverted);
}

void 
R2Calculator::calcDetAndCovarianceInverseOfNullModel(const EpisodeTagVector & episode_tags,
													 double & log_determinant,
													 boost::numeric::ublas::vector<double> & null_model_covariances_diag_inverted) const
{
	log_determinant = 0;

	null_model_covariances_diag_inverted.resize(episode_tags.size());

	size_t ret_vec_idx = 0;
	for (EpisodeTagVector::const_iterator I = episode_tags.begin(), IEnd = episode_tags.end(); I != IEnd; ++I, ++ret_vec_idx)
	{
		DBC_CHECK((*I).what() < _variances.size());

		log_determinant += log(_variances[(*I).what()]);
		null_model_covariances_diag_inverted(ret_vec_idx) = 1.0 / _variances[(*I).what()];
	}
}

void
R2Calculator::removeNaNsFromResponse(boost::numeric::ublas::vector<double> & response_min_means, size_t nans_count) const
{
	if (nans_count == 0)
	{
		return;
	}

	boost::numeric::ublas::vector<double> new_response_min_means(response_min_means.size() - nans_count);

	size_t j = 0;
	
	for (size_t i = 0; i < response_min_means.size(); ++i)
	{
		if (!infrastructure::math::isNaN(response_min_means(i)))
		{
			new_response_min_means(j++) = response_min_means(i);
		}
	}
	
	DBC_ALWAYS_CHECK(j == new_response_min_means.size());
	
	response_min_means.swap(new_response_min_means);
}

infrastructure::lazy_set<size_t>
R2Calculator::getNaNPositions(const boost::numeric::ublas::vector<double> & vector_with_nans) const
{
	infrastructure::lazy_set<size_t> nan_positions;
	for (size_t i = 0, limit = vector_with_nans.size(); i < limit; ++i)
	{
		if (infrastructure::math::isNaN(vector_with_nans(i)))
		{
			nan_positions.fast_insert(i);
		}
	}
	return nan_positions;
}
										
/// Reference: https://ncalculators.com/statistics/r-squared-calculator.htm
/// Reference: https://en.wikipedia.org/wiki/Coefficient_of_determination

double
R2Calculator::getR2() const
{
	return -_chi_2 / _samples_count;
}

double
R2Calculator::getR2NullModelHasModelsCovMat() const
{
	return -_chi_2_null_model_has_model_cov_mat / _samples_count;
}

double
R2Calculator::getR2ModelHasCovMatButReturnMeans() const
{
	return -_chi_2_true_model_response_is_mean / _samples_count;
}

void
R2Calculator::processResponsesMatrix(const boost::numeric::ublas::matrix<double> & responses_matrix)
{
	getChi2(responses_matrix);
}


void
R2Calculator::processResidualsVector(boost::numeric::ublas::vector<double> true_resp_min_mean, boost::numeric::ublas::vector<double> true_resp_min_forecast)
{
	++_samples_count;
	if (containsNaNs(true_resp_min_mean))
	{
		const infrastructure::lazy_set<size_t> & nan_positions = getNaNPositions(true_resp_min_mean);
		removeNaNsFromResponse(true_resp_min_mean, nan_positions.size());
		removeNaNsFromResponse(true_resp_min_forecast, nan_positions.size());

		if (_old_nan_positions != nan_positions)
		{//covariance submatrix changed
			calcDetAndCovarianceInverseOfModel(nan_positions,
				_model_cov_submatrix_log_determinant,
				_cov_submatrix_inverted);

			calcDetAndCovarianceInverseOfNullModel(nan_positions,
				_null_model_cov_submatrix_log_determinant,
				_null_model_submatrix_diag_inverted);

			_old_nan_positions = nan_positions;
		}

		double sumResponsesMinMeansSquaredDivByNullModelDiag = 0.0;
		for (size_t i = 0; i < _null_model_submatrix_diag_inverted.size1(); ++i)
		{
			sumResponsesMinMeansSquaredDivByNullModelDiag += 
				pow(true_resp_min_mean(i), 2) * _null_model_submatrix_diag_inverted(i, i);
		}

		_chi_2 += _model_cov_submatrix_log_determinant - _null_model_cov_submatrix_log_determinant +
			inner_prod(prod(trans(true_resp_min_forecast), _cov_submatrix_inverted), true_resp_min_forecast) -
			sumResponsesMinMeansSquaredDivByNullModelDiag;

		_chi_2_null_model_has_model_cov_mat += 
			inner_prod(prod(trans(true_resp_min_forecast), _cov_submatrix_inverted), true_resp_min_forecast) -
			inner_prod(prod(trans(true_resp_min_mean), _cov_submatrix_inverted), true_resp_min_mean);

		_chi_2_true_model_response_is_mean += _model_cov_submatrix_log_determinant - _null_model_cov_submatrix_log_determinant +
			inner_prod(prod(trans(true_resp_min_mean), _cov_submatrix_inverted), true_resp_min_mean) -
			sumResponsesMinMeansSquaredDivByNullModelDiag;
	}
	else
	{
		double sumResponsesMinMeansSquaredDivByNullModelDiag = 0.0;
		for (size_t i = 0; i < _null_model_cov_mat_diagonal.size(); ++i)
		{
			sumResponsesMinMeansSquaredDivByNullModelDiag += 
				pow(true_resp_min_mean(i), 2) / _null_model_cov_mat_diagonal[i];
		}

		_chi_2 += _cov_mat_log_determinant - _cov_mat_null_model_log_determinant +
			inner_prod(prod(trans(true_resp_min_forecast), _covariance_matrix_inverted), true_resp_min_forecast) -
			sumResponsesMinMeansSquaredDivByNullModelDiag;

		_chi_2_null_model_has_model_cov_mat += 
			inner_prod(prod(trans(true_resp_min_forecast), _covariance_matrix_inverted), true_resp_min_forecast) -
			inner_prod(prod(trans(true_resp_min_mean), _covariance_matrix_inverted), true_resp_min_mean);

		_chi_2_true_model_response_is_mean += _cov_mat_log_determinant - _cov_mat_null_model_log_determinant +
			inner_prod(prod(trans(true_resp_min_mean), _covariance_matrix_inverted), true_resp_min_mean) -
			sumResponsesMinMeansSquaredDivByNullModelDiag;
	}
}

void
R2Calculator::removeNaNEpisodes(EpisodeTagVector & episode_tags, const infrastructure::lazy_set<size_t> & nan_positions) const
{
	for (infrastructure::lazy_set<size_t>::const_reverse_iterator I = nan_positions.rbegin(), Iend = nan_positions.rend(); I != Iend; ++I)
	{
		episode_tags.erase(boost::next(episode_tags.begin(), *I));
	}
}

void
R2Calculator::processResidualsVector(boost::numeric::ublas::vector<double> true_resp_min_mean,
									 boost::numeric::ublas::vector<double> true_resp_min_forecast,
									 const boost::numeric::ublas::symmetric_matrix<double> & model_cov_mat_inverse,
									 double cov_mat_log_det,
									 EpisodeTagVector episodes)
{
	++_samples_count;

	double null_model_cov_mat_log_det = NAN_NOT_SET;

	boost::numeric::ublas::vector<double> null_model_submatrix_diag_inverted;
	
	const infrastructure::lazy_set<size_t> & nan_positions = getNaNPositions(true_resp_min_mean);
	removeNaNsFromResponse(true_resp_min_mean, nan_positions.size());
	removeNaNsFromResponse(true_resp_min_forecast, nan_positions.size());
	removeNaNEpisodes(episodes, nan_positions);
		
	calcDetAndCovarianceInverseOfNullModel(episodes, null_model_cov_mat_log_det, null_model_submatrix_diag_inverted);
	
	double sumResponsesMinMeansSquaredDivByNullModelDiag = 0.0;
	
	for (size_t i = 0; i < null_model_submatrix_diag_inverted.size(); ++i)
	{
		sumResponsesMinMeansSquaredDivByNullModelDiag += 
			pow(true_resp_min_mean(i), 2) * null_model_submatrix_diag_inverted(i);
	}

	_chi_2 += cov_mat_log_det - null_model_cov_mat_log_det +
		inner_prod(prod(trans(true_resp_min_forecast), model_cov_mat_inverse), true_resp_min_forecast) -
		sumResponsesMinMeansSquaredDivByNullModelDiag;

	_chi_2_null_model_has_model_cov_mat += 
		inner_prod(prod(trans(true_resp_min_forecast), model_cov_mat_inverse), true_resp_min_forecast) -
		inner_prod(prod(trans(true_resp_min_mean), model_cov_mat_inverse), true_resp_min_mean);

	_chi_2_true_model_response_is_mean += cov_mat_log_det - null_model_cov_mat_log_det +
		inner_prod(prod(trans(true_resp_min_mean), model_cov_mat_inverse), true_resp_min_mean) -
		sumResponsesMinMeansSquaredDivByNullModelDiag;

}

void
R2Calculator::processOLSResidualsVector(boost::numeric::ublas::vector<double> true_resp_min_mean,
										boost::numeric::ublas::vector<double> true_resp_min_forecast,
										const std::vector<double> & covariance_squared_per_what,
										const EpisodeTagVector & included_episodes)
{
	_samples_count += included_episodes.size();

	for (int i = 0; i < included_episodes.size(); ++i)
	{
		const EpisodeTag & tag = included_episodes[i];
		_chi_2 += pow(true_resp_min_forecast[i], 2) / covariance_squared_per_what[tag.what()];
		_chi_2 -= pow(true_resp_min_mean[i], 2) / covariance_squared_per_what[tag.what()];
	}
}

/// Reference: https://ncalculators.com/statistics/r-squared-calculator.htm
/// Reference: https://en.wikipedia.org/wiki/Coefficient_of_determination

double
R2Calculator::getChi2(const boost::numeric::ublas::matrix<double> & responses_matrix)
{
	DBC_REQUIRE(responses_matrix.size2() == _means.size());

	for (size_t nRow = 0; nRow < responses_matrix.size1(); ++nRow)
	{
		const boost::numeric::ublas::vector<double> & response = row(responses_matrix, nRow);
		boost::numeric::ublas::vector<double> residuals(response.size());
		for (size_t i = 0; i < residuals.size(); ++i)
		{
			residuals(i) = response(i) - _means[i];
		}
		
		processResidualsVector(residuals, residuals);
	}

	return _chi_2;
}

	}
}
