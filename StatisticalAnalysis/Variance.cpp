#include "Common.h"
#include "StatisticalAnalysis/Variance.h"
#include "Infrastructure/PredicateLogic.h"
#include "Infrastructure/MathUtils.h"
#include "DesignByContract.h"
#include "DebugNew.h"

////////////////////////////////////////////////////////////////////////////////

namespace mr
{
	namespace statistical_analysis
	{
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
using namespace infrastructure::math;
////////////////////////////////////////////////////////////////////////////////

///
/// Helper method for calculating the standard deviation
/// Reference1: http://vassarstats.net/corr_stats.html
/// Reference2: http://www.alcula.com/calculators/statistics/linear-regression/
///
double stdDev( uint32_t count, double sum, double sum_of_squares ) 
{
    double std_dev;

    if( count > 1 )
    {
        double n = static_cast<double>( count );

        std_dev = ::sqrt( std::max( 0.0, n*sum_of_squares - sum*sum ) 
                                          /  ( n * ( n - 1.0 ) ) );
    }
    else
    {
        std_dev = NAN_NA;
    }

    DBC_ALWAYS_ENSURE( implies(count>1, !isNaN(std_dev)));
    DBC_ALWAYS_ENSURE( implies(count<2,  isNaN(std_dev)));

    return std_dev;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Helper method for calculating the standard error
/// Reference1: http://www.statisticshowto.com/find-standard-error-regression-slope/
/// Reference2: http://www.alcula.com/calculators/statistics/linear-regression/
///
double stdErr( uint32_t count, double sum, double sum_of_squares )
{
    double std_err;

    if( count > 1 )
    {
        double n = static_cast<double>( count );

        std_err = ::sqrt( std::max( 0.0, sum_of_squares - sum*sum/ n ) 
                                         / ( n * ( n - 1.0 ) ) );
    }
    else
    {
        std_err = NAN_NA;
    }

    DBC_ALWAYS_ENSURE( implies(count>1, !isNaN(std_err)));
    DBC_ALWAYS_ENSURE( implies(count<2,  isNaN(std_err)));

    return std_err;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Helper method returning true if the standard deviation and standard
//  error are consistent.
/// Reference1: http://www.statisticshowto.com/find-standard-error-regression-slope/
/// Reference2: http://www.alcula.com/calculators/statistics/linear-regression/
///
bool isValidVariance( double std_deviation, double std_error, uint32_t  count )
{
    bool is_valid;

    if( count > 1 )
    {
        if( (std_deviation == 0.0)
        &&  (std_error     == 0.0) )
        {
            is_valid = true;
        }
        else if( fabs(std_deviation-std_error)  <= 1E-6)
        {
            is_valid = true;
        }
        else if( (std_deviation == std::numeric_limits<double>::infinity())
             &&  (std_error     == std::numeric_limits<double>::infinity()) )
        {
            is_valid = true;
        }
        else
        {
            const double root_n = std_deviation / std_error;

            is_valid = ( fabs((root_n * root_n) - count) < 1.0 );
        }
    }
    else
    {
        is_valid = isNaN( std_deviation ) && isNaN( std_error );
    }


    if( !is_valid)
    {
        OutputDebugString(
            (boost::format("std_deviation: %30.20f, std_error: %30.20f, count:%d\n") 
                %std_deviation %std_error %count ).str().c_str());
    }

    return is_valid;
}
////////////////////////////////////////////////////////////////////////////////
    } // namespace statistical_analysis
} // namespace mr
////////////////////////////////////////////////////////////////////////////////
