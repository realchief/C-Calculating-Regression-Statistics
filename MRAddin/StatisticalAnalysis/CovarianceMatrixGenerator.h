#ifndef MR_COVARIANCE_MATRIX_GENERATOR
#define MR_COVARIANCE_MATRIX_GENERATOR

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
template<bool with_mean>
class CovarianceMatrixGenerator
{
    typedef Vector::size_type   size_type;
public:
//  Lifetime:
    explicit CovarianceMatrixGenerator  ( size_type size );
    explicit CovarianceMatrixGenerator  ( const Vector& mean_x );

//  Commands:
    void include( const Vector& values );

//  Queries:
    SymMatrix   operator()  ( ) const;
    bool        validSample ( ) const {return _count > 0;}

private:
    size_type   _count;
    size_type   _size;
    Vector      _x;
    Matrix      _xx;
};
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor for use when means ARE NOT known in advance
///
template< > inline 
CovarianceMatrixGenerator<false>::CovarianceMatrixGenerator( size_type size )
:   _count  ( 0 ), 
    _size   ( size ),
    _x      ( ZeroVector(size) ), 
    _xx     ( ZeroMatrix( size, size ) )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor for use when means ARE known in advance
///
template< > inline 
CovarianceMatrixGenerator<true>::CovarianceMatrixGenerator( const Vector& mean_x )
:   _count  ( 0 ), 
    _size   ( mean_x.size() ),
    _x      ( mean_x ), 
    _xx     ( ZeroMatrix( mean_x.size(), mean_x.size()) )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Include the sample value vector when means ARE NOT known
///
template< > inline void 
CovarianceMatrixGenerator<false>::include( const Vector& x )
{
    ++_count;

    _x  += x;
    _xx += outer_prod(x,x);
}
////////////////////////////////////////////////////////////////////////////////

///
/// Include the sample value vector when means ARE known
///
template< > inline void 
CovarianceMatrixGenerator<true>::include( const Vector& x )
{
    ++_count;

    Vector x_diff( x - _x );

    _xx += outer_prod(x_diff,x_diff);
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return Variance-Covariance matrix when means ARE NOT known
///     (Sx²- (Sx)²/n)/n
///
template< > inline SymMatrix 
CovarianceMatrixGenerator<false>::operator()() const
{
    DBC_REQUIRE( validSample() );

    const double n = boost::numeric_cast<double>(_count);

    return SymMatrix((_xx - outer_prod(_x,_x)/n ) /n);
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return Variance-Covariance matrix when means ARE known
///     (S(x - µ)²)/n
///
template< > inline SymMatrix 
CovarianceMatrixGenerator<true>::operator()() const
{
    DBC_REQUIRE( validSample() );

    const double n = boost::numeric_cast<double>(_count);

    return _xx/n;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return correlation matrix
///
inline SymMatrix correlationMatrix( const SymMatrix& covariance )
{
    typedef SymMatrix::size_type size_type;

    const size_type size = covariance.size1();

    SymMatrix cor( size );

    for( size_type i = 0; i < size; ++i )
    {
        const double var_i = covariance(i,i);

        for( size_type j = i; j < size; ++j )
        {
            const double var_j = covariance(j,j);

            cor(i,j) = covariance(i,j) /::sqrt( var_i * var_j );
        }
    }

    return cor;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return correlation matrix
///
inline bool isValidCorrelationMatrix( const SymMatrix& correlation )
{
    typedef SymMatrix::size_type size_type;

    bool is_valid = true;

    const size_type size = correlation.size1();

    for( size_type i = 0; is_valid && i < size; ++i )
    {
        is_valid = ( correlation(i,i) <= 1.0 );

        for( size_type j = i + 1; is_valid && j < size; ++j )
        {
            is_valid = ( abs(correlation(i,j)) <= 1.0 );
        }
    }

    return is_valid;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_COVARIANCE_MATRIX_GENERATOR
////////////////////////////////////////////////////////////////////////////////
