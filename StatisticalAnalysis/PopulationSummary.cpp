#include "Common.h"
#include "StatisticalAnalysis/PopulationSummary.h"
#include "StatisticalAnalysis/StatisticsResult.h"
#include "Infrastructure/MathUtils.h"
#include "StatisticalAnalysis/FTSignificance.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace mr::infrastructure::math;
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T sqr(T x)
{
	return x * x;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
PopulationSummary::PopulationSummary( )
:   _count          ( 0 ),
    _sum            ( 0 ),
    _sum_of_squares ( 0 )
{
}
////////////////////////////////////////////////////////////////////////////////
PopulationSummary::PopulationSummary( uint32_t count, 
                                      double sum, 
                                      double sum_of_squares )
:   _count          ( count ),
    _sum            ( sum ),
    _sum_of_squares ( sum_of_squares )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
PopulationSummary::~PopulationSummary( )
{
}
////////////////////////////////////////////////////////////////////////////////
PopulationSummary& 
PopulationSummary::operator+( const PopulationSummary& other)
{
    operator+=(other);

    return *this;
}
////////////////////////////////////////////////////////////////////////////////
void 
PopulationSummary::operator+=( const PopulationSummary& other)
{
    _count          += other._count;
    _sum            += other._sum;
    _sum_of_squares += other._sum_of_squares;
}
////////////////////////////////////////////////////////////////////////////////
PopulationSummary& 
PopulationSummary::operator-( const PopulationSummary& other)
{
    operator-=(other);
    return *this;
}
////////////////////////////////////////////////////////////////////////////////
void 
PopulationSummary::operator-=( const PopulationSummary& other)
{
    _count          -= other._count;
    _sum            -= other._sum;
    _sum_of_squares -= other._sum_of_squares;
}
////////////////////////////////////////////////////////////////////////////////
void
PopulationSummary::add(double value)
{
    if( !_isnan(value) )
    {
        ++_count;

        _sum            += value;
        _sum_of_squares += ( value * value );
    }
}
////////////////////////////////////////////////////////////////////////////////
void
PopulationSummary::remove(double value)
{
    if( !_isnan(value) )
    {
        --_count;

        _sum            -= value;
        _sum_of_squares -= ( value * value );
    }
}
////////////////////////////////////////////////////////////////////////////////
void
PopulationSummary::reset( )
{
    _count          = 0;
    _sum            = 0;
    _sum_of_squares = 0;
}
////////////////////////////////////////////////////////////////////////////////
double      
PopulationSummary::mean( ) const
{
    return _count > 0 ? _sum/static_cast<double>(_count) : NAN_DIVBY0;
}
////////////////////////////////////////////////////////////////////////////////
double      
PopulationSummary::stdDeviation( ) const
{
    double std_dev;

    if( _count > 1 )
    {
        double n = static_cast<double>( _count );

        std_dev = ::sqrt( std::max( 0.0, n*_sum_of_squares - _sum*_sum ) 
                                          /  ( n * ( n - 1.0 ) ) );
    }
    else
    {
        std_dev = NAN_NA;
    }

    return std_dev;
}
////////////////////////////////////////////////////////////////////////////////
double      
PopulationSummary::stdError( ) const
{
    double std_err;

    if( _count > 1 )
    {
        double n = static_cast<double>( _count );

        std_err = ::sqrt( std::max( 0.0, _sum_of_squares - _sum*_sum/ n ) 
                                         / ( n * ( n - 1.0 ) ) );
    }
    else
    {
        std_err = NAN_NA;
    }

    return std_err;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate Welch's t test statistic (this = total != subset)
/// Reference: https://en.wikipedia.org/wiki/Welch%27s_t-test
/// Reference: https://onlinecourses.science.psu.edu/stat414/node/203
/// Reference: https://en.wikipedia.org/wiki/Student%27s_t-test
///
///
StatisticsResult    
PopulationSummary::welch_t( const PopulationSummary& subset ) const
{
    StatisticsResult result;

    PopulationSummary extra = *this;
    
    extra -= subset;

    if( (2 < subset.count())
    &&  (2 < extra.count()) )
    {
        const double subset_mean     = subset.mean();
        const double subset_std_dev  = subset.stdDeviation();
        const double subset_n        = static_cast<double>(subset.count());
        
		const double extra_mean     = extra.mean();
        const double extra_std_dev  = extra.stdDeviation();
        const double extra_n        = static_cast<double>(extra.count());

		const double extra_mean_diff 
            = subset_mean - extra_mean;

		const double t
            = extra_mean_diff 
            / ::sqrt( (sqr(subset_std_dev)/subset_n)+ (sqr(extra_std_dev)/extra_n));

		const double sig = 
		    SigOfMeanDif(   subset_mean, 
                            subset_std_dev, 
                            subset.count(), 
                            extra_mean, 
                            extra_std_dev, 
                            extra.count());

        result = StatisticsResult( t, sig );
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////
StatisticsResult    
PopulationSummary::groupRelative_F( const PopulationSummary& subset ) const
{
    StatisticsResult result;

    PopulationSummary extra = *this;
    
    extra -= subset;

    if( (2 < subset.count())
    &&  (2 < extra.count()) )
    {
        const double subset_std_dev = subset.stdDeviation();      
        const double extra_std_dev  = extra.stdDeviation();

	    double var_1 = extra_std_dev    * extra_std_dev;
	    double var_2 = subset_std_dev   * subset_std_dev;

		const double F = var_2 / var_1;

		const double sig 
            = SigOfStDevDif( subset_std_dev, 
                             subset.count(), 
                             extra_std_dev, 
                             extra.count());

        result = StatisticsResult( F, sig );
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the zero relative t statistics (this == local == total)
/// Reference: https://onlinecourses.science.psu.edu/stat414/node/203
/// Reference: http://www.sthda.com/english/wiki/welch-t-test
/// Reference: https://en.wikipedia.org/wiki/Welch%27s_t-test
/// Reference: https://en.wikipedia.org/wiki/Student%27s_t-test
///
StatisticsResult    
PopulationSummary::zeroRelative_t( ) const
{
    StatisticsResult result;

    if( 2 < count() )
    {
        const double local_mean = mean();

        const double t = local_mean / stdError(); 

		const double sig = SigOfMeanDif( local_mean, 
                                         stdDeviation(), 
                                         count(), 
                                         0.0 );

        result = StatisticsResult( t, sig );
    }

    return result;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the zero relative t statistics (local == total)
/// Reference: https://onlinecourses.science.psu.edu/stat414/node/203
/// Reference: http://www.sthda.com/english/wiki/welch-t-test
/// Reference: https://en.wikipedia.org/wiki/Welch%27s_t-test
/// Reference: https://en.wikipedia.org/wiki/Student%27s_t-test
///
StatisticsResult    
PopulationSummary::zeroRelative_F( ) const
{
    StatisticsResult result;

    return result;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
