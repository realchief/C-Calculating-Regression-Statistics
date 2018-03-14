#include "Common.h"
#include "StatisticalAnalysis/StatisticsResult.h"
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
            using mr::infrastructure::math::isNaN;
            using mr::infrastructure::math::NAN_NOT_SET;
        }
////////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
/// Reference: http://www.statisticssolutions.com/the-linear-regression-analysis-in-spss/
/// Reference: http://www.statisticssolutions.com/what-is-linear-regression/
/// Reference: https://en.wikipedia.org/wiki/Linear_regression
///
StatisticsResult::StatisticsResult()
:   _value          ( NAN_NOT_SET ),
    _mr_significance( NAN_NOT_SET ),
    _significance   ( NAN_NOT_SET )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
/// Reference: http://www.statisticssolutions.com/the-linear-regression-analysis-in-spss/
/// Reference: http://www.statisticssolutions.com/what-is-linear-regression/
/// Reference: https://en.wikipedia.org/wiki/Linear_regression
///
StatisticsResult::StatisticsResult( double value )
:   _value          ( value ),
    _mr_significance( NAN_NOT_SET ),
    _significance   ( NAN_NOT_SET )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
StatisticsResult::StatisticsResult( double value, double significance )
:   _value          ( value ),
    _mr_significance( NAN_NOT_SET ),
    _significance   ( NAN_NOT_SET )
{
    setSignificance( significance );
}
////////////////////////////////////////////////////////////////////////////////

void 
StatisticsResult::setSignificance( double significance )
{
    _significance = significance;

    if( hasValidSignificance() )
    {
        _mr_significance = 100.0 * ( 1.0 - _significance );
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return true if the value is not a NAN value
/// Reference: http://www.statisticssolutions.com/the-linear-regression-analysis-in-spss/
/// Reference: http://www.statisticssolutions.com/what-is-linear-regression/
/// Reference: https://en.wikipedia.org/wiki/Linear_regression
///
bool
StatisticsResult::isValid( ) const 
{
    return !isNaN( _value );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return true if the result has a valid significance
///
bool
StatisticsResult::hasValidSignificance( ) const 
{
    return !isNaN( _significance );
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
