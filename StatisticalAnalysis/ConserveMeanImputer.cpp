#include "Common.h"
#include "StatisticalAnalysis/ConserveMeanImputer.h"
#include "StatisticalAnalysis/Sample.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "Infrastructure/MathUtils.h"
#include "DesignByContract.h"
#include "DebugNew.h"


namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
using namespace infrastructure::math;
////////////////////////////////////////////////////////////////////////////////

///
/// Imputation functor
/// Reference1: http://www.chegg.com/homework-help/questions-and-answers/using-c-create-program-calculate-linear-model-x-y-data-pairs-using-least-squares-regressio-q4341386
/// Reference2: http://keisan.casio.com/exec/system/14059929550941
/// Reference3: https://goodcalculators.com/linear-regression-calculator/
///
void 
ConserveMeanImputer::operator()( Sample& sample )
{
    const Sample::observation_size_type variates 
        = sample.variateCount();

    _count  = ZeroVector( variates );
    _sum    = ZeroVector( variates );

    forEach( sample(), boost::bind( 
        &ConserveMeanImputer::addObservation, this, 
            _1 ) );

    Vector means = ZeroVector( variates );

    for( Sample::observation_size_type v = 0; v < variates; ++v )
    {
        if( _count(v) > 0 )
        {
            means(v) = _sum(v)/_count(v);
        }
    }

	forEach( sample(), boost::bind(
        &ConserveMeanImputer::replaceMissingValues, this,
		_1, boost::cref( means ) ) );

    sample.means() = means;
}
////////////////////////////////////////////////////////////////////////////////
///  Reference1: https://www.stata.com/statalist/archive/2011-11/msg00157.html
///  Reference2: https://www.r-bloggers.com/imputing-missing-data-with-r-mice-package/

void
ConserveMeanImputer::addObservation( const Vector& observation )
{
    for( Sample::observation_size_type v = 0, 
        last = observation.size(); v < last; ++v )
    {
        if( !isNaN( observation(v) ) )
        {
            _sum(v)     += observation(v);
            _count(v)   += 1;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void
ConserveMeanImputer::replaceMissingValues( Vector& observation, 
                                           const Vector& imputed_value ) const
{
    for( Sample::observation_size_type v = 0, 
        last = observation.size(); v < last; ++v )
    {
        if( isNaN( observation(v) ) )
        {
            observation(v) = imputed_value(v);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
