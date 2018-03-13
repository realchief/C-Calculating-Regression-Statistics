#ifndef MR_REGRESSION_RESULT_H
#define MR_REGRESSION_RESULT_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif
///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/RegressionMask.h"
#include "StatisticalAnalysis/StatisticsResult.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "DesignByContract.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost;
///////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP RegressionResult
{
public:
	typedef std::map<std::string, double> NAMED_STATISTICS_MAP;

	RegressionResult();

    RegressionResult    ( const RegressionMask& mask,
                          uint64_t observation_count,
                          uint64_t discarded_count,
                          double sum_weights,
						  bool has_constant = true);  
	//bool has_constant = true will cuase the in-sample r-squared calculation to use the in-sample mean as the null-model
	//otherwise, the null-model will be zero.

//  Queries:
    double akaike           ( ) const				{return _akaike_information_criteria;}
    double chiSquared       ( ) const				{return _chi_squared;}
    double DoF              ( ) const				{return _degrees_of_freedom;}
    double residualVariance ( ) const				{return _residual_variance;}
    double responseMean     ( ) const				{return _response_mean;}
    double responseStdDev   ( ) const				{return _response_std_dev;}
    double rSquared         ( ) const				{return _r_squared;}
    double rSquaredAdjusted ( ) const				{return _r_squared_adjusted;}
    double responseVariance ( ) const				{return _response_variance;}
    double schwarz          ( ) const				{return _schwarz_information_criteria;}
    double sumWeights       ( ) const				{return _sum_weights;}
	double rSquaredNullModelHasModelCovMat() const	{return _r_squared_null_model_has_model_cov_mat;}
	double crossValidatedRSquared()	const			{return _cross_validated_r2;}
	double rSquaredTestedModelReturnMeans() const	{return _r_squared_tested_model_returns_means;}
	double crossValidatedMSE()	const				{return _cross_validated_mse;}
	double crossValidatedLogLikelihood() const		{return _cross_validated_log_likelihood;}
	double crossValidatedLogLikelihoodWhenNullModelHasModelCovMat() const {return _cross_validated_log_likelihood_null_model_has_cov_mat;}
	double crossValidatedLogLikelihoodWhenTestedModelReturnsMeans() const {return _cross_validated_log_likelihood_tested_model_returns_means;}

	double inSampleMSE() const  {return _in_sample_mse; }
	double inSampleLogLikelihood() const  {return _in_sample_log_likelihood; }
	double inSampleLogLikelihoodWhenNullModelHasModelCovMat() const {return _in_sample_log_likelihood_null_model_has_cov_mat;}
	double inSampleLogLikelihoodWhenTestedModelReturnsMeans() const {return _in_sample_log_likelihood_tested_model_returns_means;}

	bool hasConstant() const { return _has_constant; }

//  Navigable associations
    const RawResults&           beta                ( ) const {return _beta;}
	RawResults&                 beta                ( )       {return _beta;} //non-const version added to support intercept scaling after calculation
    const RawResults&           betaStdErr          ( ) const {return _beta_standard_error;}
    const StatisticsResults&    tBeta               ( ) const {return _t_beta;}
    const StatisticsResult&     fRatio              ( ) const {return _f_ratio;}
    const RegressionMask&       mask                ( ) const {return _mask;}
    const RawResults&           predictorMeans      ( ) const {return _predictor_means;}
    const RawResults&           predictorStdDevs    ( ) const {return _predictor_std_devs;}
    const SymMatrix&            betaCovariance      ( ) const {return _beta_covariance;}
	SymMatrix&					betaCovariance      ( )	      {return _beta_covariance;}
	//const SymMatrix&            xtX                 ( ) const {return _xtx_matrix;}  //X'*X

	const RawResults&           coefficients        ( ) const {return _coefficients;}
	RawResults&                 coefficients        ( )       {return _coefficients;} //non-const version added to support intercept scaling after calculation
	const RawResults&           coefficientsOLS     ( ) const {return _coefficients_ols;}
	const RawResults&			SOLSAlpha			( ) const { return _sols_alpha;}
	const Matrix &				SOLSSpaceBaseChangeMatrix() const {return _sols_space_base_change_matrix;}

	const NAMED_STATISTICS_MAP& namedStatistics		( ) const {return _named_statistics;}
	const std::vector<std::string> & switchedPredictorNames () const  { return _switched_predictor_names; }
	const std::vector<std::string> & prunedPredictorNames () const  { return _pruned_predictor_names; }

	const std::vector<uint32_t>& pruneGroupId       ( ) const {return _prune_group_id;}

//  Auxiliary Queries:
    uint64_t observationCount   ( )             const {return _observation_count;}
    uint64_t discardedCount     ( )             const {return _discarded_count;}
    uint32_t predictorCount     ( )             const {return _mask.size();}
    double  predictorMean       ( size_t p )    const;
    double  predictorStdDev     ( size_t p )    const;
    bool    inversionFailed     ( )             const {return _inversion_failed;}

//	Commands:	
	void setRSquared            ( double value )                    {_r_squared                     = value;}
	void setRSquaredNullModelHasModelCovMat(double value)			{_r_squared_null_model_has_model_cov_mat = value;}
	void setRSquaredTestedModelReturnMeans(double value)			{_r_squared_tested_model_returns_means = value;}
	void setCrossValidatedRSquared	( double value )				{_cross_validated_r2				= value;}

	void setCrossValidatedMSE	( double value )					{_cross_validated_mse				= value;}
	void setCrossValidatedLogLikelihood( double value )				{_cross_validated_log_likelihood	= value;}
	void setCrossValidatedLogLikelihoodWhenNullModelHasModelCovMat( double value ) {_cross_validated_log_likelihood_null_model_has_cov_mat = value;}
	void setCrossValidatedLogLikelihoodWhenTestedModelReturnsMeans( double value ) {_cross_validated_log_likelihood_tested_model_returns_means = value;}

	void setInSampleMSE( double value )										 {_in_sample_mse = value;}
	void setInSampleLogLikelihood( double value )							 {_in_sample_log_likelihood	= value;}
	void setInSampleLogLikelihoodWhenNullModelHasModelCovMat( double value ) {_in_sample_log_likelihood_null_model_has_cov_mat = value;}
	void setInSampleLogLikelihoodWhenTestedModelReturnsMeans( double value ) {_in_sample_log_likelihood_tested_model_returns_means = value;}

    void setAkaike              ( double value )                    {_akaike_information_criteria   = value;}
    void setBeta                ( const RawResults& values );       //code now in RegressionResult.cpp (to support conditional initialization of switch pruning results)
    void setBetaCovariance      ( const SymMatrix& covariance )     {_beta_covariance               = covariance;}
	//void setXtX                 ( const SymMatrix& xtx )            {_xtx_matrix                    = xtx;}
    void setBetaStdErr          ( const RawResults& values )        {_beta_standard_error           = values;}
    void setChiSquared          ( double value )                    { _chi_squared                  = value;}
    void setDoF                 ( double value )                    { _degrees_of_freedom           = value;}
    void setFRatio              ( const StatisticsResult& result )  {_f_ratio                       = result;}
    void setInversionFailed     ( bool failed )                     {_inversion_failed              = failed;}
    void setResidualVariance    ( double value )                    {_residual_variance             = value;}
    void setRSquaredAdjusted    ( double value )                    {_r_squared_adjusted            = value;}
    void setSchwarz             ( double value )                    {_schwarz_information_criteria  = value;}
    void setTBeta               ( const StatisticsResults& values ) {_t_beta                        = values;}
    void setPredictorMeans      ( const RawResults& values )        {_predictor_means               = values;}
    void setPredictorStdDevs    ( const RawResults& values )        {_predictor_std_devs            = values;}
    void setResponseMean        ( double value )                    {_response_mean                 = value;}
    void setResponseStdDev      ( double value )                    {_response_std_dev              = value;}
    void setResponseVariance    ( double value )                    {_response_variance             = value;}
	void setObservationCount	( uint64_t value )				    {_observation_count				= value;}
    void setDiscardedCount      ( int value )                       {_discarded_count               = value;}
	void setSumWeights			( double value )					{_sum_weights					= value;}
	void setCoefficients        ( const RawResults& values )        {_coefficients                  = values;}
	void setCoefficientsOLS     ( const RawResults& values )        {_coefficients_ols              = values;}
	void setSOLSAlpha			( const RawResults& values )		{_sols_alpha					= values;}
	void setSOLSSpaceBaseChangeMatrix( const Matrix & space_base_change_matrix){ if (space_base_change_matrix.size1() > 0) _sols_space_base_change_matrix = space_base_change_matrix; }//assigning empty matrix to empty matrix causes CRT assertion

	void setNamedStatistic(const std::string & statistic_name, double value) { _named_statistics.insert(std::make_pair(statistic_name, value)); }

	void setSwitchedPredictorNames (const std::vector<std::string> & switched_predictor_names)  { _switched_predictor_names = switched_predictor_names; }
	void setPrunedPredictorNames (const std::vector<std::string> & pruned_predictor_names)  { _pruned_predictor_names = pruned_predictor_names; }

	void setPruneGroupId        ( const std::vector<uint32_t>& prune_group_id) { _prune_group_id = prune_group_id; }

private:
//  Input used to generate result
    RegressionMask  _mask;
    uint64_t        _observation_count;
    uint64_t        _discarded_count;
    double          _sum_weights;

//  True if calculator was unable to invert X matrix
    bool    _inversion_failed;

//  Results:
    double              _akaike_information_criteria;
    RawResults          _beta;
    SymMatrix           _beta_covariance;
	//SymMatrix           _xtx_matrix;
    RawResults          _beta_standard_error;
    double              _chi_squared;
    double              _degrees_of_freedom;
    StatisticsResult    _f_ratio;
    double              _residual_variance;
    double              _r_squared;
    double              _r_squared_adjusted;
    double              _schwarz_information_criteria;
    StatisticsResults   _t_beta;
    RawResults          _predictor_means;
    RawResults          _predictor_std_devs;
    double              _response_mean;
    double              _response_std_dev;
    double              _response_variance;
	double				_r_squared_null_model_has_model_cov_mat;
	double				_r_squared_tested_model_returns_means;

	double              _cross_validated_r2;
	double				_cross_validated_mse;
	double				_cross_validated_log_likelihood;
	double				_cross_validated_log_likelihood_null_model_has_cov_mat;
	double				_cross_validated_log_likelihood_tested_model_returns_means;

	double				_in_sample_mse;
	double				_in_sample_log_likelihood;
	double				_in_sample_log_likelihood_null_model_has_cov_mat;
	double				_in_sample_log_likelihood_tested_model_returns_means;

	RawResults			_coefficients;
	RawResults          _sols_alpha;
	Matrix				_sols_space_base_change_matrix;

	RawResults			_coefficients_ols;

	NAMED_STATISTICS_MAP _named_statistics;

	std::vector<std::string> _switched_predictor_names;
	std::vector<std::string> _pruned_predictor_names;

	std::vector<uint32_t> _prune_group_id;
	bool                  _has_constant;
};
///////////////////////////////////////////////////////////////////////////////
inline double
RegressionResult::predictorMean( size_t p ) const
{
    DBC_REQUIRE( p < _predictor_means.size() );
    return _predictor_means[p];
}
///////////////////////////////////////////////////////////////////////////////
inline double
RegressionResult::predictorStdDev( size_t p ) const
{
    DBC_REQUIRE( p < _predictor_std_devs.size() );
    return _predictor_std_devs[p];
}
///////////////////////////////////////////////////////////////////////////////

    }   // namespace statistical_analysis
}   //  namespace mr
///////////////////////////////////////////////////////////////////////////////
#endif  // MR_REGRESSION_RESULT_H
///////////////////////////////////////////////////////////////////////////////

