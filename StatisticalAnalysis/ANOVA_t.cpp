////////////////////////////////////////////////////////////////////////////////
#include "Common.h"
#include "StatisticalAnalysis/ANOVA_t.h"
#include "StatisticalAnalysis/Variance.h"
#include "FTSignificance.h"
#include "DesignByContract.h"
#include "Infrastructure/MathUtils.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
ANOVA_t::ANOVA_t( uint32_t total_count, 
                  double total_sum, 
                  double total_sum_sq)
:   _total_count    ( total_count ),
    _total_sum      ( total_sum ),
    _total_sum_sq   ( total_sum_sq )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
/// 
ANOVA_t::~ANOVA_t( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the multiple group result for the given local statistics
/// Reference1:  http://www.bmj.com/about-bmj/resources-readers/publications/statistics-square-one/11-correlation-and-regression
/// Reference2:  https://blackboard.jhu.edu/bbcswebdav/pid-3918442-dt-content-rid-17483493_2/courses/NR.120.508.0101.SP17/120.508%20Module%208%20Multiple%20Regression%20%28PDF%20Full%20page%20color%29.pdf
///
StatisticsResult    
ANOVA_t::multipleGroupResult( uint32_t local_count, 
                              double local_sum, 
                              double local_sum_sq,
                              double local_std_dev,
                              double local_std_err) const
{
    StatisticsResult result;

	const uint32_t extra_count = _total_count - local_count;

    if( (2 < local_count)
    &&  (2 < extra_count) )
    {
        const double local_mean
            = local_sum / static_cast<double>( local_count );

        const double sum_diff
            = _total_sum - local_sum;

        const double sum_sq_diff    
            = _total_sum_sq - local_sum_sq;

		const double extra_mean
            = sum_diff / static_cast<double>( extra_count );

		const double extra_mean_diff 
            = local_mean - extra_mean;

        const double extra_std_dev = stdDev( 
            extra_count, sum_diff, sum_sq_diff );

        const double extra_std_err = stdErr( 
            extra_count, sum_diff, sum_sq_diff );

		double t = NAN_NA;
		double sig = NAN_NA;

		if (isValidVariance(extra_std_dev, extra_std_err, extra_count ))
		{
			t = extra_mean_diff 
                        / ::sqrt((local_std_err*local_std_err)+(extra_std_err*extra_std_err));

			sig = SigOfMeanDif( local_mean, 
                                            local_std_dev, 
                                            local_count, 
                                            extra_mean, 
                                            extra_std_dev, 
                                            extra_count);
		}
        result = StatisticsResult( t, sig );
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the single group result for the given local statistics
/// Reference1: http://www.stat.yale.edu/Courses/1997-98/101/linmult.htm
/// Reference2: http://mezeylab.cb.bscb.cornell.edu/labmembers/documents/supplement%205%20-%20multiple%20regression.pdf
/// Reference3: http://www.stat.yale.edu/Courses/1997-98/101/linmult.htm
///
StatisticsResult    
ANOVA_t::singleGroupResult( uint32_t local_count, 
                            double local_sum, 
                            double local_std_dev,
                            double local_std_err) const
{
    StatisticsResult result;

    if( 2 < local_count )
    {
        const double local_mean = local_sum 
                                / static_cast<double>( local_count );

        const double t = local_mean 
                       / local_std_err; 

		const double sig = SigOfMeanDif( local_mean, 
                                         local_std_dev, 
                                         local_count, 
                                         0.0 );

        result = StatisticsResult( t, sig );
    }

    return result;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
