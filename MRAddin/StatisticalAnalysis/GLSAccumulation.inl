#include "StatisticalAnalysis/Observation.h"
#include "DesignByContract.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace boost;
///////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
///
template <typename ContainersConfiguration>
inline basic_gls_accumulation<ContainersConfiguration>::basic_gls_accumulation( )
{
    setBatchSize(0);
}
///////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
template <typename ContainersConfiguration>
inline basic_gls_accumulation<ContainersConfiguration>::basic_gls_accumulation( uint32_t predictor_count )
:   basic_accumulation<ContainersConfiguration>( predictor_count )
{
    setBatchSize(0);
}
///////////////////////////////////////////////////////////////////////////////

///
/// (Re)Set the batch size
///
template <typename ContainersConfiguration>
inline void
basic_gls_accumulation<ContainersConfiguration>::setBatchSize( size_t batch_size )
{
    _observation_k = 0;
    _batch_size = batch_size;

    _X = ZeroMatrix( _batch_size, predictors() );
    _Y = ZeroVector( _batch_size );
}
///
/// (Re)Set the batch size
///
template <>
inline void
basic_gls_accumulation<switched_models_common::SparseContainers>::setBatchSize( size_t batch_size )
{
	_observation_k = 0;
	_batch_size = batch_size;

	//workaround for msvc2010 debug error - cannot handle _batch_size == 0
	//_X = switched_models_common::SparseContainers::matrix_type( _batch_size, predictors() );
	if (batch_size == 0)
	{
        switched_models_common::SparseContainers::matrix_type x(_batch_size, predictors());
		_X.swap(x);
	}
	else
	{
		_X = switched_models_common::SparseContainers::matrix_type( _batch_size, predictors() );
	}
	_Y = ZeroVector( _batch_size );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Accumulate x and y, and build the X matrix and Y vector
///
template <typename ContainersConfiguration>
inline void 
basic_gls_accumulation<ContainersConfiguration>::accumulate( const basic_observation<ContainersConfiguration> & observation )
{
    const Response& response = observation.response();
    const Vector& predictors = observation.predictors();

    typedef boost::numeric::ublas::matrix_row<Matrix> MatrixRow;

    _Y(_observation_k) = response();

    MatrixRow row(_X, _observation_k); 

    row = predictors;

    _sum_x  += predictors;
    _sum_y  += response();

    ++_sum_weights;
    ++_observations;
    ++_observation_k;
}
///////////////////////////////////////////////////////////////////////////////
template <>
inline void 
basic_gls_accumulation<switched_models_common::SparseContainers>::accumulate( const basic_observation<switched_models_common::SparseContainers> & observation )
{
	const Response& response = observation.response();
	const switched_models_common::SparseContainers::vector_type & predictors = observation.predictors();

	_Y(_observation_k) = response();

	for (switched_models_common::SparseContainers::vector_type::const_iterator I = predictors.begin(), Iend = predictors.end(); I != Iend; ++I)
	{
		_X(_observation_k, I.index()) = (*I);
		_sum_x(I.index()) += (*I);
	}

	_sum_y  += response();

	++_sum_weights;
	++_observations;
	++_observation_k;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Multiply the X matrix and Y vector by the covariance inverse
///
template <typename ContainersConfiguration>
inline void 
basic_gls_accumulation<ContainersConfiguration>::accumulateBatch( const SymMatrix& covariance_inverse )
{
    DBC_REQUIRE( covariance_inverse.size1() == _batch_size );

    const Matrix cov_inv_XT( trans(prod(covariance_inverse, _X)) );

    _sum_yy += inner_prod(prod(covariance_inverse,_Y), _Y);
    _sum_xy += prod(cov_inv_XT,_Y);
    _sum_xx += prod(cov_inv_XT,_X);
}
///////////////////////////////////////////////////////////////////////////////
template <>
inline void 
basic_gls_accumulation<switched_models_common::SparseContainers>::accumulateBatch( const SymMatrix& covariance_inverse )
{
	DBC_REQUIRE(_X.size1() == covariance_inverse.size1());
	DBC_REQUIRE(_Y.size() == covariance_inverse.size1());

	//const switched_regression::SparseContainers::matrix_type cov_inv_XT(
	//	trans(boost::numeric::ublas::sparse_prod<switched_regression::SparseContainers::matrix_type>(covariance_inverse, _X)) );
	switched_models_common::SparseContainers::matrix_type cov_inv_XT(_X.size2(), _X.size1());

	for (switched_models_common::SparseContainers::matrix_type::const_iterator2 I = _X.begin2(), Iend = _X.end2(); I != Iend; ++I)
	{
		const size_t i_index = I.index2();
	
		//ublas way to iterate through filled indexes only
		for (switched_models_common::SparseContainers::matrix_type::const_iterator1 J = I.begin(), Jend = I.end(); J != Jend; ++J)
		{
			const size_t j_index = J.index1();
			double sum = 0.0;
			
			for (switched_models_common::SparseContainers::matrix_type::const_iterator1 K = I.begin(), Kend = I.end(); K != Kend; ++K)	
			{
				sum += (*K) * covariance_inverse(j_index, K.index1());
			}
			cov_inv_XT(i_index, j_index) = sum;
		}
	}

	_sum_yy += inner_prod(prod(covariance_inverse,_Y), _Y);
	
	//_sum_xy += prod(cov_inv_XT, _Y);
	for (switched_models_common::SparseContainers::matrix_type::const_iterator1 I = cov_inv_XT.begin1(), Iend = cov_inv_XT.end1(); I != Iend; ++I)
	{
		double sum = 0.0;
		//ublas way to iterate through filled indexes only
		for (switched_models_common::SparseContainers::matrix_type::const_iterator2 J = I.begin(), Jend = I.end(); J != Jend; ++J)
		{
			sum += (*J) * _Y(J.index2());
		}
		_sum_xy(I.index1()) += sum;
	}

	//_sum_xx += prod(cov_inv_XT,_X);
	switched_models_common::SparseContainers::matrix_type tmp_m(_sum_xx.size1(), _sum_xx.size2());
	sparse_prod(cov_inv_XT,_X, tmp_m, false);
	
	//_sum_xx += tmp_m;
	for (switched_models_common::SparseContainers::matrix_type::const_iterator1 I = tmp_m.begin1(), Iend = tmp_m.end1(); I != Iend; ++I)
	{
		const size_t i_index = I.index1();
		for (switched_models_common::SparseContainers::matrix_type::const_iterator2 J = I.begin(), Jend = I.end(); J != Jend; ++J)
		{
			if (i_index >= J.index2())
			{
				_sum_xx(i_index, J.index2()) += (*J);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
