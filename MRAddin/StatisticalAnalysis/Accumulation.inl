
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace infrastructure::math;
using namespace boost;
///////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
///
template <typename ContainersConfiguration>
inline basic_accumulation<ContainersConfiguration>::basic_accumulation(  )
:   _predictors( 0 )
{
    reset();
}
///////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
template <typename ContainersConfiguration>
inline basic_accumulation<ContainersConfiguration>::basic_accumulation( uint32_t predictor_count )
:   _predictors( predictor_count )
{
    reset();
}
///////////////////////////////////////////////////////////////////////////////

///
/// Reset everything [except the predictor count]
///
template <typename ContainersConfiguration>
inline void 
basic_accumulation<ContainersConfiguration>::reset( )
{
    _observations   = 0;
    _sum_weights    = 0.0;
    _sum_y          = 0.0;
    _sum_yy         = 0.0;
    _sum_xx         = ZeroMatrix(_predictors, _predictors);
    _sum_x          = ZeroVector(_predictors);
    _sum_xy         = ZeroVector(_predictors);
}

///
/// Multiply everything by the given factor
///
template <typename ContainersConfiguration>
inline void 
basic_accumulation<ContainersConfiguration>::operator*=( double factor )
{
    if( (factor != 1.0) && (_observations > 0) )
    {
        _sum_weights *= factor;
        _sum_y       *= factor;
        _sum_yy      *= factor;

        _sum_x  *= factor;
        _sum_xy *= factor;
        _sum_xx *= factor;
    }
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return the mean of the specified predictor
/// 
template <typename ContainersConfiguration>
inline double
basic_accumulation<ContainersConfiguration>::predictorMean( uint32_t p ) const
{
    DBC_REQUIRE( p < _predictors );

    return ( _observations > 0 )
        ?   _sum_x[p] / _observations
        :   NAN_DIVBY0;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return the standard deviation of the specified predictor
/// 
template <typename ContainersConfiguration>
inline double
basic_accumulation<ContainersConfiguration>::predictorStdDev( uint32_t p ) const
{
    DBC_REQUIRE( p < _predictors );

    return stdDev( numeric_cast<uint32_t>(_observations), _sum_x[p], _sum_xx(p,p) );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return the mean of the responses
/// 
template <typename ContainersConfiguration>
inline double
basic_accumulation<ContainersConfiguration>::responseMean( ) const
{
    return ( _observations > 0 )
        ?   sumY() / _observations
        :   NAN_DIVBY0;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return the standard deviation of the responses
/// 
template <typename ContainersConfiguration>
inline double
basic_accumulation<ContainersConfiguration>::responseStdDev( ) const
{
    return stdDev( numeric_cast<uint32_t>(_observations), _sum_y, _sum_yy );
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
