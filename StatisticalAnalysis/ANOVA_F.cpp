////////////////////////////////////////////////////////////////////////////////
#include "Common.h"
#include "StatisticalAnalysis/ANOVA_F.h"
#include "StatisticalAnalysis/PopulationSummary.h"
#include "StatisticalAnalysis/Synopsis.h"
#include "StatisticalAnalysis/FTSignificance.h"
#include "DesignByContract.h"
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
ANOVA_F::ANOVA_F( )
:   _group_count    ( 0 ),
    _sum_sq_between ( 0.0 ),
    _sum_sq_within  ( 0.0 ),
    _total_count    ( 0 ),
    _total_sum      ( 0.0 )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
/// 
ANOVA_F::~ANOVA_F( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Add a sample to the accumulation
/// Reference1: http://www.chegg.com/homework-help/questions-and-answers/using-c-create-program-calculate-linear-model-x-y-data-pairs-using-least-squares-regressio-q4341386
/// Reference2: https://support.sas.com/edu/schedules.html?ctry=ca&crs=STAT2#s1=1,
/// Reference3: https://mxnet.incubator.apache.org/tutorials/sparse/train.html,
/// Reference4: https://www.southampton.ac.uk/passs/gcse_scores/multivariate_analysis/linear_regression.page
///
void
ANOVA_F::addGroupSynopsis( const Synopsis& synopsis )
{
    addGroup( synopsis.count(), synopsis.sum(), synopsis.sumOfSquares() );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Add a sample to the accumulation
///
void 
ANOVA_F::addGroup( const PopulationSummary& population )
{
    addGroup( population.count(), population.sum(), population.sumOfSquares() );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Add a group to the accumulation
///
void
ANOVA_F::addGroup( uint32_t count, double sum, double sum_sq )
{
    if( count )
    {
        const double mean = sum/static_cast<double>(count);
	    _sum_sq_within	+= sum_sq - mean * sum;
	    _sum_sq_between	+= sum * sum/static_cast<double>(count);
        ++_group_count;

        _total_count += count;
        _total_sum   += sum;
    }
}
////////////////////////////////////////////////////////////////////////////////
void 
ANOVA_F::calculateResult()
{
    calculateResult( _total_count, _total_sum );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the result for the given total statistics
///
void
ANOVA_F::calculateResult( uint32_t total_count, 
                          double total_sum )
{
    const double ss_within 
        = _sum_sq_within;

    const double ss_between 
        = _sum_sq_between 
        - total_sum * total_sum / static_cast<double>(total_count);

	const int df_within	
        = total_count - _group_count;

	const int df_between	
        = _group_count - 1;

    if( ( df_within  > 0 ) && ( df_between  > 0 )
    &&  ( ss_within != 0 ) && ( ss_between != 0 ) )
    {
		const double variance_within  = ss_within  / df_within;
		const double variance_between = ss_between / df_between;

		double f, df_1, df_2;

		if (variance_within > variance_between)
		{
			f    = variance_within / variance_between;
			df_1 = df_between;
			df_2 = df_within;
		}
		else
		{
			f    = variance_between / variance_within;
			df_1 = df_within;
			df_2 = df_between;
		}

        double sig = FSig( f, df_1, df_2 );

        if( sig > 0.5 )sig = 1.0 - sig;

        _result = StatisticsResult( variance_between / variance_within, sig );                                    
    }
    else
    {
        _result = StatisticsResult();
    }
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
