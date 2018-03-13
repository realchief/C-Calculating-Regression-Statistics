#ifndef MR_STATISTICAL_ANALYSIS_MATRIX_DEFINITIONS_H
#define MR_STATISTICAL_ANALYSIS_MATRIX_DEFINITIONS_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
typedef boost::numeric::ublas::vector<double>           Vector;
typedef boost::numeric::ublas::zero_vector<double>      ZeroVector;
typedef boost::numeric::ublas::matrix<double>           Matrix;
typedef boost::numeric::ublas::zero_matrix<double>      ZeroMatrix;
typedef boost::numeric::ublas::identity_matrix<double>  IdentityMatrix;
typedef boost::numeric::ublas::symmetric_matrix<double> SymMatrix;

//template boost::numeric::ublas::vector<double>;
////////////////////////////////////////////////////////////////////////////////
    } // namespace statistical_analysis
} // namespace mr
////////////////////////////////////////////////////////////////////////////////

#endif  // MR_STATISTICAL_ANALYSIS_MATRIX_DEFINITIONS_H
////////////////////////////////////////////////////////////////////////////////

