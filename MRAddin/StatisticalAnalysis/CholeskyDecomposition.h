#ifndef STATYSTICAL_ANALYSIS_CHOLESKY_DECOMPOSITION_H_INCLUDED
#define STATYSTICAL_ANALYSIS_CHOLESKY_DECOMPOSITION_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#pragma warning(push,1)
#pragma warning(disable : 4267 4702)
#endif

#include "DesignByContract.h"

namespace mr
{
	namespace statistical_analysis
	{
		/*
		Performs Cholesky matrix decomposition. The input matrix must be:
		1. Symmertic
		2. Positive definite (please see http://en.wikipedia.org/wiki/Positive-definite_matrix )

		The function is based on http://trix.cs.ucla.edu/jenni/NR/Cpp211.cpp/recipes/choldc.cpp
		modified by A.S. to support ublas matrices. See http://www.nrbook.com/a/bookcpdf/c2-9.pdf for details.

		L matrix can be used for precise calculation of decomposed matrix determinant, like that
		//symmetric, positive definite matrix
		symmetric_matrix<double> M(3, 3);
		triangular_matrix<double, lower> L(3, 3);	
		choleskyDecomposition(M, L);

		det = pow(L(0, 0) * L(1, 1) * L(2, 2), 2.0);

		here is why:
		1. M == L * trans(L) //that is what Cholesky decomposition does
		2. always det(A) == det(trans(A)), where A is a matrix N x N
		3. always det(A*B) == det(A) * det(B) where A and B is a matrices N x N
		4. So, from (3) we know det(M) == det(L) * det(trans(L))
		5. from (2) we know det(L) == det(trans(L))
		6. so, finally det(M) == pow(det(L), 2.0)
		*/
		template<typename T>
		void
		choleskyDecomposition(const boost::numeric::ublas::symmetric_matrix<T> & matrix_to_decompose,
							  boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> & result_matrix)
		{
			//we know, that M.size1() == M.size2(), so there is no use of making DBC
			
			typedef typename boost::numeric::ublas::matrix<T>::size_type size_type;

			const size_type matrix_size = matrix_to_decompose.size1();

			boost::numeric::ublas::matrix<T> M(matrix_to_decompose);
			boost::numeric::ublas::vector<T> P(matrix_size);
			
			for (size_type i = 0; i < matrix_size; ++i)
			{
				for (size_type j = i; j < matrix_size; ++j)
				{
					double sum = M(i, j);
					for (size_type k = 0; k < i; ++k)
					{
						sum -= M(i, k) * M(j, k);
					}

					if (i == j)
					{
						if (sum <= 0.0)
						{
							throw std::runtime_error("Cholesky decomposition error, the matrix is not positive definite.");
						}
						P(i) = sqrt(sum);
					}
					else
					{
						M(j, i) = sum / P(i);
					}
				}
			}

			result_matrix = boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower>(matrix_size, matrix_size);

			for (size_type i = 0; i < matrix_size; ++i)
			{
				result_matrix(i, i) = P(i);
				for (size_t j = 0; j < i; ++j)
				{
					result_matrix(i, j) = M(i, j);
				}
			}
		}
		
		/*
		This function is provides convenient way to invert cholesky factorization result matrix L.
		Using L inversion we can easily calculate inversion of the original matrix. Please see the example:
		
		//symmetric, positive definite matrix
		symmetric_matrix<double> M(3, 3);
		triangular_matrix<double, lower> L(3, 3);	

		choleskyDecomposition(M, L);
		triangular_matrix<double, upper> L_inv(3, 3);	
		choleskyInvert(L, L_inv);
		
		now, we have
		M^(-1) = L_inv * trans(L_inv)

		from http://trix.cs.ucla.edu/jenni/NR/Cpp211.cpp/recipes/choldc.cpp
		see http://www.nrbook.com/a/bookcpdf/c2-9.pdf
		*/
		template<typename T>
		void choleskyInvert(boost::numeric::ublas::triangular_matrix<T, boost::numeric::ublas::lower> /* no reference here, we modify it! */ L,
							boost::numeric::ublas::triangular_matrix<T, boost::numeric::ublas::upper> & L_inv)
		{
			DBC_REQUIRE(L.size1() == L.size2());
			
			typedef typename boost::numeric::ublas::matrix<T>::size_type size_type;
			const size_type matrix_size = L.size1();

			L_inv = boost::numeric::ublas::triangular_matrix<T, boost::numeric::ublas::upper>(matrix_size, matrix_size);	
			for (size_type i = 0; i < matrix_size; ++i)
			{
				L_inv(i, i) = L(i, i) = 1.0 / L(i, i);
				for (size_type j = i + 1; j < matrix_size; ++j)
				{
					double sum = 0.0;
					for (size_type k = i; k < j; ++k)
					{
						sum -= L(j, k) * L(k, i);
					}
					//yes, L_inv(i, j) = L(j, i)
					L_inv(i, j) = L(j, i) = sum / L(j, j);
				}
			}
		}
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
