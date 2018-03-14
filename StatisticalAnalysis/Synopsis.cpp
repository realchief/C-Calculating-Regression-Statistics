#include "Common.h"
#include "StatisticalAnalysis/Synopsis.h"
#include "StatisticalAnalysis/Variance.h"
#include "Infrastructure/MathUtils.h"
#include "MRMath.h"
#include "DesignByContract.h"
#include "DebugNew.h"

////////////////////////////////////////////////////////////////////////////////

namespace mr
{
	namespace statistical_analysis
	{
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
using namespace std;
using namespace infrastructure::math;
////////////////////////////////////////////////////////////////////////////////
inline bool even(uint64_t value) { return (value & 1) == 0;}
////////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
///
Synopsis::Synopsis( )
:   _counters( NUMBER_OF_COUNTERS )
{
    initialise( );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Copy constructor
///
Synopsis::Synopsis( const Synopsis& source )
{
    operator=( source );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
Synopsis::~Synopsis( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Assignment operator
/// Reference1: http://nasacj.net/?p=189
/// Reference2: https://www.geeksforgeeks.org/linear-regression-python-implementation/
///
void
Synopsis::operator=( const Synopsis& rhs )
{
    _counters               = rhs._counters;
    _grp_rel_t              = rhs._grp_rel_t;
    _maximum                = rhs._maximum;
    _median                 = rhs._median;
    _minimum                = rhs._minimum;
    _sum                    = rhs._sum;
    _sum_abs                = rhs._sum_abs;
    _sum_squares            = rhs._sum_squares;
    _unique_value_count     = rhs._unique_value_count;
    _values                 = rhs._values;
    _sorted                 = rhs._sorted;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the counter for the index
/// Reference: https://octave.sourceforge.io/optim/function/LinearRegression.html
///
uint32_t 
Synopsis::counter( uint32_t  index ) const
{
    DBC_REQUIRE( index < NUMBER_OF_COUNTERS );

    return _counters[index];
}
////////////////////////////////////////////////////////////////////////////////

///
/// Initialise the analysis
/// Reference: https://octave.sourceforge.io/optim/function/LinearRegression.html
///
void
Synopsis::initialise( )
{
    for( uint32_t  counter = 0; counter < NUMBER_OF_COUNTERS; ++counter )
    {
        _counters[counter] = 0;
    }

    _maximum            = NAN_NOT_SET;
    _minimum            = NAN_NOT_SET;
    _sum                = NAN_NOT_SET;
    _sum_squares        = NAN_NOT_SET;
    _sum_abs            = NAN_NOT_SET;

    _grp_rel_t.reset();
    _mean.reset();
    _median.reset();
    _unique_value_count.reset();

    _sorted  = false;
    _values.clear();
}
///////////////////////////////////////////////////////////////////////////////

///
/// Analyse the value, and include in appropriate accumulators
/// Reference http://nasacj.net/?p=189
///
void
Synopsis::addValue( double value )
{
    if( isNaN( value ) )
    {
        accumulateNaN( getNaNType(value) );
    }
    else
    {
        accumulateValue( value );
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Increment the appropriate counter for the given NAN
/// Reference https://octave.sourceforge.io/optim/function/LinearRegression.html
///
void
Synopsis::accumulateNaN( int nan_type )
{
	if( ( nan_type & DO_SET ) != 0 )
    {
        ++_counters[ COUNT_NAN_DOSET ];
    }
	else if( ( nan_type & INVALID ) != 0 )
    {
        ++_counters[ COUNT_NAN_INVALID ];
    }
	else if( ( nan_type & DIVBY0 ) != 0 )
    {
        ++_counters[ COUNT_NAN_DIVBY0 ];
    }
	else if( ( nan_type & NOT_SET ) != 0 )
    {
        ++_counters[ COUNT_NAN_NOT_SET ];
    }
	else if( ( nan_type & NA ) != 0 )
    {
        ++_counters[ COUNT_NAN_NA ];
    }
    else
    {
        ++_counters[ COUNT_NAN_UNKNOWN ];
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Accumulate value
/// https://octave.sourceforge.io/optim/function/LinearRegression.html
///
void
Synopsis::accumulateValue( double value )
{
    if( count() == 0 )
    {
		_maximum	    = value;
		_minimum	    = value;
        _sum            = value;
        _sum_abs        = fabs(value);
        _sum_squares    = value * value;
    }
    else
    {
        _maximum        = std::max( _maximum, value );
        _minimum        = std::min( _minimum, value );
        _sum            +=   value;
        _sum_abs        += fabs(value);
        _sum_squares    += ( value * value );
    }

    if( value < 0 )
    {
        ++_counters[ COUNT_VALUE_NEGATIVE ];
    }
    else if( value > 0 )
    {
        ++_counters[ COUNT_VALUE_POSITIVE ];
    }
    else
    {
        ++_counters[ COUNT_VALUE_ZERO ];
    }

    _values.push_back( value );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Calculate and cache Welch's t test value given the summary of all rows
/// Reference https://www.mathworks.com/help/stats/linearmodel-class.html?requestedDomain=true
///
void
Synopsis::setGrpRel_t( const Synopsis& all_rows )
{
    double grp_rel_t = NAN_NA;

	const uint32_t  extra_count = all_rows.count() - count();

    if( ( count() >= 3 ) && ( extra_count >= 3 ) )
	{
        const double extra_sum
            = all_rows.sum() - sum();

        const double extra_sum_squares
            = all_rows.sumOfSquares() - sumOfSquares();

		const double extra_mean         
            = extra_sum / extra_count;

		const double extra_mean_diff	
            = *mean() - extra_mean;

        const double extra_std_deviation  
            = stdDev( extra_count, extra_sum, extra_sum_squares );

        const double extra_std_error      
            = stdErr( extra_count, extra_sum, extra_sum_squares );

        DBC_ALWAYS_CHECK( isValidVariance( 
            extra_std_deviation, extra_std_error, extra_count ) );

        const double std_error = standardError( );

        grp_rel_t 
            = extra_mean_diff 
            /::sqrt((std_error*std_error)+(extra_std_error*extra_std_error));
	}

    _grp_rel_t = Statistic( new double( grp_rel_t ) );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the mean of the result rows analysed
///
void
Synopsis::calculateMean( ) 
{
    setMean( (count() > 0) ? sum()/count() : NAN_DIVBY0 );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return the mean / standard deviation
/// Reference1: https://en.wikipedia.org/wiki/Standard_deviation
/// Reference2: http://www.lboro.ac.uk/media/wwwlboroacuk/content/mlsc/downloads/mean_median_mode.pdf
///
double
Synopsis::meanPerStdDev( ) const
{
    double result;

    if( count() > 0 )
    {
        if( count() > 1 )
        {
            double s = standardDeviation( );

            result = ( s > 0.0 ) ? *mean() / s : NAN_DIVBY0;
        }
        else
        {
            result = NAN_NA;
        }
    }
    else
    {
        result = NAN_NOT_SET;
    }

    return result;
}
/////////////////////////////////////////////////////////////////////

///
/// Return the mean / standard error (t-stat)
/// Reference: http://davidmlane.com/hyperstat/A103735.html
/// Reference: http://www.statisticshowto.com/what-is-the-standard-error-of-a-sample/
///
double
Synopsis::meanPerStdErr( ) const
{
    double result;

    if( count() > 0 )
    {
        if( count() > 1 )
        {
            double s = standardError( );

            result = ( s > 0.0 ) ? *mean() / s : NAN_DIVBY0;
        }
        else
        {
            result = NAN_NA;
        }
    }
    else
    {
        result = NAN_NOT_SET;
    }

    return result;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the number of NAN results 
///
uint32_t 
Synopsis::nanCount( ) const
{
    return  _counters[ COUNT_NAN_DOSET ]
         +  _counters[ COUNT_NAN_DIVBY0 ]
         +  _counters[ COUNT_NAN_INVALID ]
         +  _counters[ COUNT_NAN_NA ]
         +  _counters[ COUNT_NAN_NOT_SET ]
         +  _counters[ COUNT_NAN_UNKNOWN ];
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the standard-deviation of the results
/// Reference1: https://www.khanacademy.org/math/probability/data-distributions-a1/summarizing-spread-distributions/a/calculating-standard-deviation-step-by-step
/// Reference2: https://en.wikipedia.org/wiki/Standard_deviation
/// Reference3: https://www.mathsisfun.com/data/standard-deviation-formulas.html
///
///
double
Synopsis::standardDeviation( ) const
{
    double standard_deviation 
        = stdDev( count(), sum(), sumOfSquares() );

    if( 
        !isValidVariance( 
            standard_deviation, 
                stdErr( count(), sum(), sumOfSquares() ), count() ) )
    {
// If this is a windows platform then output a warning to the debug window
// explaining that the variance is invalid.
#ifdef _WIN32
        OutputDebugString("Synopsis::standardDeviation has failed to produce a valid variance.");
#endif // ifdef _WIN32

        standard_deviation = NAN_NA;
    }

    return standard_deviation;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Return the standard-error of the results
/// Reference: http://davidmlane.com/hyperstat/A103735.html
/// Reference: http://www.statisticshowto.com/what-is-the-standard-error-of-a-sample/
///
double
Synopsis::standardError( ) const
{
    const double standard_error 
        = stdErr( count(), sum(), sumOfSquares() );

    DBC_ALWAYS_ENSURE( 
        isValidVariance( 
            stdDev( count(), sum(), sumOfSquares() ), 
                standard_error, count() ) );

    return standard_error;
}
////////////////////////////////////////////////////////////////////////////////
double              
Synopsis::meanAbs( ) const 
{
    return count() > 0 
        ? _sum_abs / boost::numeric_cast<double>(count())
        : NAN_DIVBY0;
}
////////////////////////////////////////////////////////////////////////////////
double              
Synopsis::meanSquares( ) const 
{
    return count() > 0 
        ? _sum_squares / boost::numeric_cast<double>(count())
        : NAN_DIVBY0;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Update the median value of the rows
/// Reference: https://stackoverflow.com/questions/40319985/how-to-calculate-mean-values-from-a-linear-model-in-r
/// Reference: https://rstudio-pubs-static.s3.amazonaws.com/119859_a290e183ff2f46b2858db66c3bc9ed3a.html
/// Reference: https://www1.udel.edu/johnmack/frec424/regression/
///
void
Synopsis::calculateMedian( )
{
    if( sorted() == false )
    {
        sort();
    }

    const uint64_t count = _values.size();

    if( count )
    {
        if( even( count ) )
        {
//          If there are an even number of values, 
//            take the average of the central pair
            const uint32_t index = numeric_cast<uint32_t>( count/2 );

            setMedian( 0.5*( _values[index] + _values[index-1] ) );
        }
        else
        {
            const uint32_t index = numeric_cast<uint32_t>( (count-1)/2 );

            setMedian( _values[index] );
        }
    }
    else
    {
        setMedian( NAN_NA );
    }
}
////////////////////////////////////////////////////////////////////////////////

///
/// Sort the values
///
void
Synopsis::sort( )
{
    std::sort( _values.begin(), _values.end() );
    _sorted = true;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Sort the values
///
void
Synopsis::calculateUniqueValueCount( )
{
    if( isNull(_unique_value_count) )
    {
        std::vector<double> unique_values = _values;

        std::vector<double>::iterator new_end 
            = std::unique( unique_values.begin(), unique_values.end() );

        unique_values.erase( new_end, unique_values.end() );

        setUniqueValueCount( 
            numeric_cast<uint32_t>( 
                unique_values.size() ) );
    }
}
std::string         
Synopsis::formatted( const std::string& sep ) 
{
    std::stringstream result;   

    calculateMean();
    calculateMedian();

    result  << "Min, Max: "  << "["  << minimum() << "," << maximum() << "]"    << sep
            << "Mean____: "  << *mean() << " �" << standardError()              << sep
            << "StdDev__: "  << standardDeviation()                             << sep
            << "Median__: "  << *median()                                       << sep
            << "Count___: "  << count()                                         << sep
            << "# < 0___: "  << countNegative()                                 << sep
            << "# = 0___: "  << countZero()                                     << sep
            << "# > 0___: "  << countPositive();


    return result.str();
}
////////////////////////////////////////////////////////////////////////////////
    } // namespace statistical_analysis
} // namespace mr
////////////////////////////////////////////////////////////////////////////////
