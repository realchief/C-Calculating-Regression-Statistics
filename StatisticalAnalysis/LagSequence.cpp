#include "Common.h"
#include "StatisticalAnalysis/LagSequence.h"
#include "StatisticalAnalysis/Episode.h"
#include "StatisticalAnalysis/Sample.h"
#include "StatisticalAnalysis/SortPredicates.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "Infrastructure/Loop.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
////////////////////////////////////////////////////////////////////////////////
typedef lazy_set<Episode*, CongruousPred<Episode*> > Sequence;
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
LagSequence::LagSequence( Episodes& episodes )
:   _episodes (episodes)
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
LagSequence::~LagSequence( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Build the lag sequence by manipulating the sample's episodes
/// Reference: http://www.statisticssolutions.com/assumptions-of-linear-regression/
/// Reference: https://en.wikipedia.org/wiki/Regression_validation
///
void
LagSequence::build( )
{
    if( !_episodes.empty() )
    {
        Sequence sequence;

        forEach( _episodes, boost::bind( 
            &Sequence::fast_insert, 
                &sequence, boost::bind( toPtr, _1 ) ) );

        Sequence::const_iterator next 
            = sequence.begin();

        ++next;

        for( Sequence::const_iterator episode = sequence.begin(),
                last_epsisode = sequence.end(); 
                    next != last_epsisode; ++episode, ++next )
        {
            (*episode)->buildLagSequence(*next);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void 
LagSequence::useAllObservations( )
{
    forEach( _episodes, boost::bind(
        &Episode::useAllObservations, _1));
}
////////////////////////////////////////////////////////////////////////////////
void 
LagSequence::useOnlyCommonPredictors( bool by_record, const Lags& lags )
{
    if( by_record )
    {
        forEach( _episodes, boost::bind(
            &Episode::useOnlyCommonPredictorsByRecord, _1, 
                boost::cref(lags)));
    }
    else
    {
        forEach( _episodes, boost::bind(
            &Episode::useOnlyCommonPredictors, _1, 
                boost::cref(lags)));
    }
}
////////////////////////////////////////////////////////////////////////////////
void 
LagSequence::useOnlyCommonResponses( bool by_record, const Lags& lags )
{
    if( by_record )
    {
        forEach( _episodes, boost::bind(
            &Episode::useOnlyCommonResponsesByRecord, _1, 
                boost::cref(lags)));
    }
    else
    {
        forEach( _episodes, boost::bind(
            &Episode::useOnlyCommonResponses, _1, 
                boost::cref(lags)));
    }
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
