#ifndef SCRIPT_TOOLS_MATRIX_LU_INVERTER_H_INCLUDED
#define SCRIPT_TOOLS_MATRIX_LU_INVERTER_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#pragma warning(push,1)
#pragma warning(disable : 4267)
#endif

/*
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <functional>
*/

#include "cancelable_lu.hpp"
#include "MatrixInversionCanceled.h"
#include "DesignByContract.h"

namespace mr
{
	namespace statistical_analysis
	{
		/*
			Inverts matrices using LU factorization method.
			Throws std::invalid_argument for not invertible matrices.
		*/

		template<typename T>
		void matrixLUInvert(const boost::numeric::ublas::matrix<T> & m, boost::numeric::ublas::matrix<T> & inv)
		{
				DBC_ALWAYS_REQUIRE(m.size1() == m.size2());
				boost::numeric::ublas::matrix<T> mat(m);

				const boost::numeric::ublas::matrix<T>::size_type n_th_row_processing_error = boost::numeric::ublas::lu_factorize(mat);
				
				if (n_th_row_processing_error != 0)
				{
					throw std::invalid_argument("Matrix cannot be inverted. Singularity detected.");
				}

				inv = boost::numeric::ublas::identity_matrix<T>(mat.size1());

				boost::numeric::ublas::lu_substitute<boost::numeric::ublas::matrix<T> const, boost::numeric::ublas::matrix<T> >(mat, inv);
		}

		template<typename T>
		void cancelableMatrixLUInvert(const boost::numeric::ublas::matrix<T> & m, boost::numeric::ublas::matrix<T> & inv,
			infrastructure::CancelQuery cancel_query, infrastructure::ProgressSink progress_sink)
		{
			DBC_ALWAYS_REQUIRE(m.size1() == m.size2());
			boost::numeric::ublas::matrix<T> mat(m);

			const boost::numeric::ublas::matrix<T>::size_type n_th_row_processing_error 
				= boost::numeric::ublas::lu_cancelable_factorize(mat, cancel_query, progress_sink);

			if (cancel_query() == true)
			{
				throw MatrixInversionCanceled();
			}

			if (n_th_row_processing_error != 0)
			{
				throw std::invalid_argument("Matrix cannot be inverted. Singularity detected.");
			}

			inv = boost::numeric::ublas::identity_matrix<T>(mat.size1());

			boost::numeric::ublas::lu_substitute<boost::numeric::ublas::matrix<T> const, boost::numeric::ublas::matrix<T> >(mat, inv);
		}

		template<typename T>
		class MatrixLUInverter : public std::unary_function< const boost::numeric::ublas::matrix<T> &, boost::numeric::ublas::matrix<T> >
		{
		public:
			explicit MatrixLUInverter(infrastructure::CancelQuery cancel_query, infrastructure::ProgressSink progress_sink)
				: _cancel_query(cancel_query), _progress_sink(progress_sink)
			{
			}
			
			MatrixLUInverter()
				: _cancel_query(), _progress_sink()
			{
			}

			boost::numeric::ublas::matrix<T> operator() (const boost::numeric::ublas::matrix<T> & m) const
			{
				boost::numeric::ublas::matrix<T> inv;
				if (_cancel_query && _progress_sink)
				{
					cancelableMatrixLUInvert(m, inv, _cancel_query, _progress_sink);
				}
				else
				{
					matrixLUInvert(m, inv);
				}
				return inv;
			}
		private:
			infrastructure::CancelQuery _cancel_query;
			infrastructure::ProgressSink _progress_sink;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
