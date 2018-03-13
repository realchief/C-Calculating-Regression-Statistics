#ifndef MR_STATISTICAL_ANALYSIS_MATRIX_SVD_INVERTER_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_MATRIX_SVD_INVERTER_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

#pragma warning (push, 0)
#pragma warning (disable : 6246)
#include "MrMath.h"
#pragma warning (pop)

#include "MatrixInversionCanceled.h"
/*#include <boost/numeric/bindings/lapack/gesdd.hpp>
#include <boost/numeric/bindings/traits/matrix_traits.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>*/

namespace mr
{
	namespace statistical_analysis
	{
		template<typename T>
		class MatrixSVDInverter
		{
		public:
			MatrixSVDInverter()
				: _cancel_query(infrastructure::DummyCancel()),
				 _progress_sink(infrastructure::DummyProgress())
			{
			}

			MatrixSVDInverter(infrastructure::CancelQuery cancel_query, infrastructure::ProgressSink progress_sink)
				: _cancel_query(cancel_query),
				  _progress_sink(progress_sink)
			{
			}

			boost::numeric::ublas::matrix<T> operator() (const boost::numeric::ublas::matrix<T> & m) const
			{
				/*boost::numeric::ublas::matrix<T, boost::numeric::ublas::column_major> A(m);
				boost::numeric::ublas::matrix<T, boost::numeric::ublas::column_major> U(A.size1(), A.size1());
				boost::numeric::ublas::vector<T> W(std::min(A.size1(), A.size2()), 0.0);
				boost::numeric::ublas::matrix<T, boost::numeric::ublas::column_major> V(A.size2(), A.size2());
				boost::numeric::bindings::lapack::gesdd(A, W, U, V);

				boost::numeric::ublas::matrix<T, boost::numeric::ublas::column_major> uTrans(trans(U));

				for (size_t i = 0; i < W.size(); ++i)
				{
					boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<double, boost::numeric::ublas::column_major>> uRow(uTrans, i);
					double dW = W[i];
					if (dW != 0.0)
					{
						uRow *= 1.0 / dW;
					}
					else
					{
						uRow *= 0.0;
					}
				}
				
				return prod(trans(V), uTrans);*/
				
				using namespace ::mr::math;

				DBC_ALWAYS_REQUIRE(m.size1() == m.size2());
				
				const size_type matrix_size = boost::numeric_cast<size_type>(m.size1());

				CMatrix<T> andrews_matrix(matrix_size, matrix_size);
				for (size_type row = 0; row < matrix_size; ++row)
				{
					for (size_type col = 0; col < matrix_size; ++col)
					{
						andrews_matrix(row, col) = m(row, col);
					}
				}
				
				const CMatrix<T> & M_inv = andrews_matrix.Inverse(_cancel_query, _progress_sink);
				
				if (_cancel_query())
				{
					throw MatrixInversionCanceled();
				}

				return toUblasMatrix(M_inv);
			}

			void SVDDecompose(const boost::numeric::ublas::matrix<T> & m,
							  boost::numeric::ublas::matrix<T> & u, 
							  boost::numeric::ublas::diagonal_matrix<T> & sigma,
							  boost::numeric::ublas::matrix<T> & v_trans)
			{
				using namespace ::mr::math;
				
				CMatrix<T> andrews_matrix(m.size1(), m.size2());
				for (size_type row = 0; row < m.size1(); ++row)
				{
					for (size_type col = 0; col < m.size2(); ++col)
					{
						andrews_matrix(row, col) = m(row, col);
					}
				}
				
				DMAT mdUsvd;
				DMAT mdVsvd;
				DVEC vdWsvd;
				DMAT mdThisMat = andrews_matrix.ConvertTo<double>();

				// Decompose the matrix into A = U ^ Diag(W) ^ Trans(V)
				mdThisMat.SVD(mdUsvd, vdWsvd, mdVsvd, _cancel_query, _progress_sink);
				
				if (_cancel_query())
				{
					throw MatrixInversionCanceled();
				}

				u = toUblasMatrix(mdUsvd);
				sigma = toUblasMatrix(vdWsvd);
				v_trans = toUblasMatrix(mdVsvd);
			}

		private:
			static
			boost::numeric::ublas::matrix<T>
			toUblasMatrix(const ::mr::math::CMatrix<T> & andrews_matrix)
			{
				boost::numeric::ublas::matrix<T> ret(andrews_matrix.NumRows(), andrews_matrix.NumColumns());
				for (typename boost::numeric::ublas::matrix<T>::size_type i = 0; i < ret.size1(); ++i)
				{
					for (typename boost::numeric::ublas::matrix<T>::size_type j = 0; j < ret.size2(); ++j)
					{
						ret(i, j) = andrews_matrix(i, j);
					}
				}
				return ret;
			}

			static
			boost::numeric::ublas::diagonal_matrix<T>
			toUblasMatrix(const ::mr::math::DVEC & vec)
			{
				boost::numeric::ublas::diagonal_matrix<T> ret(vec.size());

				for (typename boost::numeric::ublas::diagonal_matrix<T>::size_type i = 0; i < ret.size1(); ++i)
				{
					ret(i, i) = vec(i);
				}
				return ret;
			}


		private:
			infrastructure::CancelQuery _cancel_query;
			infrastructure::ProgressSink _progress_sink;
		};
	}
}

#endif //MR_STATISTICAL_ANALYSIS_MATRIX_SVD_INVERTER_H_INCLUDED
