#include "Common.h"
#include "StatisticalAnalysis/CorrelationResult.h"
#include "StatisticalAnalysis/FTSignificance.h"
#include "Infrastructure/MathUtils.h"
#include "DesignByContract.h"
#include "DebugNew.h"

////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
        namespace 
        {
            using mr::infrastructure::math::NAN_NA;
            using mr::infrastructure::math::isNaN;
            using boost::numeric_cast;
            using boost::uint32_t;
        }          
////////////////////////////////////////////////////////////////////////////////
const double STD_DEV_TO_CI_95 = 1.959963984540054;
////////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
///
CorrelationResult::CorrelationResult( )
:   _ci_lower_bound ( NAN_NA ),
    _ci_upper_bound ( NAN_NA ),
    _count          ( 0 )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
CorrelationResult::CorrelationResult( double correlation, uint64_t count )
:   StatisticsResult( correlation ),
    _ci_lower_bound ( NAN_NA ),
    _ci_upper_bound ( NAN_NA ),
    _count          ( count )
{
    if( isValid() )
    {
        calculateSignificance( );
        calculateConfidenceInterval( );
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
CorrelationResult::CorrelationResult( double correlation, 
                                      double significance,
                                      uint64_t count )
:   StatisticsResult( correlation, significance ),
    _ci_lower_bound ( NAN_NA ),
    _ci_upper_bound ( NAN_NA ),
    _count          ( count )
{
    if( isValid() )
    {
        calculateConfidenceInterval( );
    }
}////////////////////////////////////////////////////////////////////////////////

///
/// Return true if this is a valid correlation result
///
bool
CorrelationResult::isValid( ) const
{
    return StatisticsResult::isValid( ) && ( abs( value( ) ) <= 1.0 );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the probability that the correlation of two series is significant 
//  based on Fisher's z transformation and the students t-test from NR page 639
///
void 
CorrelationResult::calculateSignificance( )
{
    if( _count > 2 )
    {
        const double correlation = value();

	    const double degrees_freedom 
            = static_cast<double>( _count - 2 );

	    const double t  
            = correlation 
            * sqrt( degrees_freedom 
            / ( (1.0 - correlation + DBL_MIN) * (1.0 + correlation + DBL_MIN)) );

	    setSignificance( TSig( t, degrees_freedom ) );
    }
    else
    {
        setSignificance( 0.0 );
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the 95% Confidence Interval for the correlation based on Fisher's 
//  z transformation and the students t-test from NR page 617
///
void 
CorrelationResult::calculateConfidenceInterval()
{
    if( _count <= 3 )
    {
        _ci_lower_bound = -1.0;
        _ci_upper_bound = +1.0;
        return;
    }

    const double correlation = value();

	if( correlation == -1.0 )
    {
        _ci_lower_bound = -1.0;
        _ci_upper_bound = -1.0;
    }
	else if( correlation == 1.0 )
    {
        _ci_lower_bound = 1.0;
        _ci_upper_bound = 1.0;
    }
    else
    {
	    const double z = 0.5 * log( (1.0 + correlation + DBL_MIN) 
                                  / (1.0 - correlation + DBL_MIN) );

    	const double z_ci_95 = STD_DEV_TO_CI_95 
                               / sqrt(static_cast<double>(_count-3));

	    const double z_lo = z - z_ci_95;
    	const double z_hi = z + z_ci_95;

	    const double exp_2_z_lo = exp(2.0 * z_lo);
	    const double exp_2_z_hi = exp(2.0 * z_hi);

	    _ci_lower_bound = (exp_2_z_lo - 1.0)/(1.0 + exp_2_z_lo);
	    _ci_upper_bound = (exp_2_z_hi - 1.0)/(1.0 + exp_2_z_hi);
    }
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
