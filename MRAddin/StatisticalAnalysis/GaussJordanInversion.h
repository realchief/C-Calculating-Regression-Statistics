#ifndef MR_GAUSE_JORDAN_INVERSION_H
#define MR_GAUSE_JORDAN_INVERSION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "DesignByContract.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
class GaussJordanInversionFailure 
    : public std::exception
{
public:
    GaussJordanInversionFailure()
        :exception("Failed to invert matrix using Gauss-Jordan algorithm")
    {}
};
////////////////////////////////////////////////////////////////////////////////
template<class T>
class GaussJordanInversion 
    : public std::unary_function< const boost::numeric::ublas::matrix<T>&, 
                                        boost::numeric::ublas::matrix<T> >
{
    typedef boost::numeric::ublas::matrix<T>            Matrix;
    typedef boost::numeric::ublas::zero_matrix<T>       ZeroMatrix;
    typedef boost::numeric::ublas::matrix_range<Matrix> MatrixRange;
    typedef boost::numeric::ublas::matrix_row<Matrix>   MatrixRow;
    typedef typename Matrix::size_type                  size_type;

public:
///////////////////////////////////////////////////////////////////////////////

///
/// Gauss Jordan Inversion functor
///
Matrix operator()( const Matrix& m )
{
    DBC_REQUIRE_DESCRIPTION( m.size1() == m.size2(), 
        "Matrix to be inverted is not square!");

    DBC_REQUIRE_DESCRIPTION( isValid(m),
        "Matrix to be inverted contains invalid values!");

    _special_case       = false;
    _result_infinite    = false;
    
    handleSpecialCases(m);

    if( !_special_case )
    {
        addIdentity(m);
        eliminateZeros(m);
        partialPivot(m);
    }

    if( !_special_case 
    //&&  !_result_infinite
    &&  !isValidInverse(m) )
    {
        throw GaussJordanInversionFailure();
    }

    return _result;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return true if the matrix is valid (Does not contain NaN, Infinite, Indefinite)
///
bool isValid( const Matrix& m )
{
    bool is_valid = true;

    const size_type size = m.size1();

    for( size_type i = 0; is_valid && i < size; ++i )
    {
        for( size_type j = 0; is_valid && j < size; ++j )
        {
            const double value = m(i,j);

            is_valid = _finite(value) && !_isnan(value);
        }
    }
    return is_valid;
}
///////////////////////////////////////////////////////////////////////////////
bool isResultInfinite   ( ) const{return _result_infinite;}
bool isSpecialCase      ( ) const{return _special_case;}
///////////////////////////////////////////////////////////////////////////////

///
/// Return true if the result is the inverse of m
///
bool isValidInverse( const Matrix& m ) const
{
    const Matrix identity = prod( m, _result );

    bool is_identity = true;

    const size_type size = m.size1();

    for( size_type i = 0; is_identity && i < size; ++i )
    {
        for( size_type j = 0; is_identity && j < size; ++j )
        {
            is_identity = ( i == j )
                ?   abs(identity(i,j)-1.) < 1E-6
                :   abs(identity(i,j))    < 1E-6;
        }
    }
    return is_identity;
}

private:
///////////////////////////////////////////////////////////////////////////////

///
/// Initialise the result = [ M | I ]
///
void addIdentity( const Matrix& m )
{
    const size_type size = m.size1();

    _result = ZeroMatrix(size, 2*size);

    MatrixRange left( _result, 
                        boost::numeric::ublas::range(0,size),
                        boost::numeric::ublas::range(0,size) );
    left = m;

    _right.reset( new MatrixRange( _result, 
                    boost::numeric::ublas::range(0,size), 
                    boost::numeric::ublas::range(size,2*size) ));

    *_right = boost::numeric::ublas::identity_matrix<T>(size);
}
///////////////////////////////////////////////////////////////////////////////

///
/// Eliminate zeros on the original matrix diagonal
///
void eliminateZeros( const Matrix& m )
{
    const size_type size = m.size1();

    for(size_type k = 0; k < size; k++)
    {
        if( _result(k,k) == static_cast<T>(0) )
        {
//          Find a row (l) to swap with row (k)
            size_type l = size;
            for( size_type i = k+1; i < size; i++)
            {
                if( _result(i,k) != static_cast<T>(0) )
                {
                    l = i;
                    break;
                }
            }
            DBC_ALWAYS_CHECK_DESCRIPTION( 
                l != size, 
                    "Input Matrix is singular" );

            MatrixRow rowk(_result, k);
            MatrixRow rowl(_result, l);
            rowk.swap(rowl);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////

///
/// Do the main work of inverting the matrix [I | 1/M]
///
void partialPivot( const Matrix& m )
{
    _result_infinite = false;

    const size_type size = m.size1();
    const size_type twice_size = 2 * size;

    for( size_type k = 0; k < size; ++k )
    {
        const double pivot = _result(k,k);

        if( tooSmall(pivot) )_result_infinite = true;

//      Normalize current row
        for( size_type j = k + 1; j < twice_size; ++j )
        {
			//_clear87();
            _result(k,j) /= pivot;
			/*if (_status87() == 0)
			{
				_result_infinite = true;
			}*/
        }
        _result(k,k) = static_cast<T>(1);

//      Normalize other rows
        for( size_type i = 0; i < size; ++i )
        {
            if( i != k && _result(i,k) != static_cast<T>(0) )
            {
                for( size_type j = k + 1; j < twice_size; ++j )
                {
                    _result(i,j) -= (_result(k,j) * _result(i,k));
                }
                _result(i,k) = static_cast<T>(0);
            }
        }
    }

    _result = *_right;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Handle special input cases 
///
void handleSpecialCases( const Matrix& m)
{
    const size_type size = m.size1();
    if( size == 0 )
    {
        _special_case = true;
    }
    else if( size == 1 )
    {
        DBC_ALWAYS_CHECK_DESCRIPTION( 
            m(0,0) != static_cast<T>(0) , 
                "Input Matrix is singular" );
        _result.resize(1,1);
        _result(0,0) = 1 / m(0,0);
        _special_case = true;
    }
}
////////////////////////////////////////////////////////////////////////////////
    bool tooSmall( double value )           const {return abs(value) < 1E-12;}
////////////////////////////////////////////////////////////////////////////////
    bool tooSmall( boost::uint64_t value )  const {return (value == 0);}
////////////////////////////////////////////////////////////////////////////////
    Matrix                          _result;
    boost::shared_ptr<MatrixRange>  _right;
    bool                            _special_case;
    bool                            _result_infinite;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_GAUSS_JORDAN_INVERSION
////////////////////////////////////////////////////////////////////////////////
