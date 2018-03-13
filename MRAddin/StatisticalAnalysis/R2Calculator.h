#ifndef MR_STATISTICAL_ANALYSIS_R_2_CALCULATOR_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_R_2_CALCULATOR_H_INCLUDED

#include "StatisticalAnalysis/API.h"
#include "EpisodeTag.h"

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

namespace mr
{
    namespace statistical_analysis
    {
		class STATISTICAL_ANALYSIS_EXPIMP R2Calculator : boost::noncopyable
		{
		public:
			R2Calculator(const std::vector<double> & response_means,
						 const std::vector<double> & response_variance);

			R2Calculator(const boost::numeric::ublas::symmetric_matrix<double> & covariances,
						 const std::vector<double> & response_means);

			R2Calculator(const boost::numeric::ublas::symmetric_matrix<double> & covariances,
				const std::vector<double> & response_means,
				const std::vector<double> & response_variance);

			void processResidualsVector(boost::numeric::ublas::vector<double> true_resp_min_mean,/*by value - we modify the copy*/
										boost::numeric::ublas::vector<double> true_resp_min_forecast);

			void processResidualsVector(boost::numeric::ublas::vector<double> true_resp_min_mean,
										boost::numeric::ublas::vector<double> true_resp_min_forecast,
										const boost::numeric::ublas::symmetric_matrix<double> & model_cov_mat_inverse,
										double cov_mat_log_det,
										EpisodeTagVector episodes);

			void processOLSResidualsVector(boost::numeric::ublas::vector<double> true_resp_min_mean,
										   boost::numeric::ublas::vector<double> true_resp_min_forecast,
										   const std::vector<double> & covariance_squared_per_what,
										   const EpisodeTagVector & included_episodes);

			void processResponsesMatrix(const boost::numeric::ublas::matrix<double> & responses_matrix);

			double getR2() const;
			double getR2NullModelHasModelsCovMat() const;
			double getR2ModelHasCovMatButReturnMeans() const;

		private:
			double getChi2(const boost::numeric::ublas::matrix<double> & responses_matrix);

			bool containsNaNs(boost::numeric::ublas::vector<double> & response_min_means) const;
			
			boost::numeric::ublas::symmetric_matrix<double> 
				getReOrderedMatrix(const boost::numeric::ublas::symmetric_matrix<double> & matrix_to_reorder, 
				const infrastructure::lazy_set<size_t> & nan_positions) const;
			
			void calcDetAndCovarianceInverseOfModel(const infrastructure::lazy_set<size_t> & nan_entries_indices,
													double & determinant,
													boost::numeric::ublas::symmetric_matrix<double> & covariances_inverted) const;

			void calcDetAndCovarianceInverseOfNullModel(const infrastructure::lazy_set<size_t> & nan_entries_indices,
														double & determinant,
														boost::numeric::ublas::diagonal_matrix<double> & null_model_cov_mat_diag_inverted) const;

			void calcDetAndCovarianceInverseOfNullModel(const EpisodeTagVector & episode_tags,
														double & determinant,
														boost::numeric::ublas::vector<double> & null_model_cov_mat_diag_inverted) const;

			void removeNaNsFromResponse(boost::numeric::ublas::vector<double> & response_min_means, size_t nans_count) const;
			void removeNaNEpisodes(EpisodeTagVector & episode_tags, const infrastructure::lazy_set<size_t> & nan_positions) const;

			infrastructure::lazy_set<size_t> getNaNPositions(const boost::numeric::ublas::vector<double> & vector_with_nans) const;

			double choleskyDecompositionBasedLogDet(const boost::numeric::ublas::symmetric_matrix<double> & the_matrix,
													boost::numeric::ublas::symmetric_matrix<double> & inv_matrix) const;

			void calcDetAndCovarianceInverseOfDiagonalCovMat(const infrastructure::lazy_set<size_t> & nan_entries_indices,
															 const std::vector<double> & cov_mat_diagonal,
															 double & log_determinant,
															 boost::numeric::ublas::diagonal_matrix<double> & null_model_covariances_diag_inverted) const;

		private:
			boost::numeric::ublas::symmetric_matrix<double> _covariance_matrix;
			boost::numeric::ublas::symmetric_matrix<double> _covariance_matrix_inverted;
			const std::vector<double> _means;
			const std::vector<double> _variances;

			double _cov_mat_log_determinant;
			double _cov_mat_null_model_log_determinant;
			std::vector<double> _null_model_cov_mat_diagonal;
			infrastructure::lazy_set<size_t> _universal_index_set;
			
			double _chi_2;
			double _model_cov_submatrix_log_determinant;
			double _null_model_cov_submatrix_log_determinant;
			boost::numeric::ublas::symmetric_matrix<double> _cov_submatrix_inverted;
			boost::numeric::ublas::diagonal_matrix<double> _null_model_submatrix_diag_inverted;
			infrastructure::lazy_set<size_t> _old_nan_positions;

			size_t _samples_count;

			double _chi_2_null_model_has_model_cov_mat;
			double _chi_2_true_model_response_is_mean;
		};
	}
}

#endif //MR_STATISTICAL_ANALYSIS_R_2_CALCULATOR_H_INCLUDED
