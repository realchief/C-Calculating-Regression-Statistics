#ifndef MR_STATISTICAL_ANALYSIS_DETERMINANT_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_DETERMINANT_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#pragma warning(push,1)
#pragma warning(disable : 4267 4702)
#endif

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <functional>
#include "DesignByContract.h"

namespace mr
{
	namespace statistical_analysis
	{
		/*
			Calculates matrices' determinant using LU factorization method.
		*/
		template<typename T>
		class Determinant : public std::unary_function< const boost::numeric::ublas::matrix<T> &, double >
		{
		public:
			double operator() (const boost::numeric::ublas::matrix<T> & m) const
			{
				DBC_ALWAYS_REQUIRE(m.size1() == m.size2());
				
				boost::numeric::ublas::permutation_matrix<size_t> ipiv(m.size1());

				boost::numeric::ublas::matrix<T> mat(m);

				const boost::numeric::ublas::matrix<T>::size_type n_th_row_processing_error = boost::numeric::ublas::lu_factorize(mat, ipiv);
				if (n_th_row_processing_error != 0)
				{
					return 0.0;
				}

				double det = 1.0;
				for (size_t i = 0, cnt = ipiv.size(); i < cnt; ++i)
				{
					if (ipiv(i) != i)
					{
						det *= -1;
					}
					det *= mat(i, i);
				}

				return det;
			}
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
