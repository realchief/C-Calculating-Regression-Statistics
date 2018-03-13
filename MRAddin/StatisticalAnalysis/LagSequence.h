#ifndef MR_STATISTICAL_ANALYSIS_LAG_SEQUENCE_H
#define MR_STATISTICAL_ANALYSIS_LAG_SEQUENCE_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "Infrastructure/LazySet.h"
#include "StatisticalAnalysis/Episode.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP LagSequence
{
    typedef vector<Episode> Episodes;

public:
//  Lifetime:
    explicit    LagSequence ( Episodes& episodes );
               ~LagSequence ( );

//  Commands:
    void build                  ( );
    void useAllObservations     ( );
    void useOnlyCommonPredictors( bool by_record, const Lags& lags );
    void useOnlyCommonResponses ( bool by_record, const Lags& lags );

//  Queries:

private:
//  Associations:
    Episodes& _episodes;


//  Not Implemented
    LagSequence     ( const LagSequence& );
    void operator=  ( const LagSequence& );
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_LAG_SEQUENCE_H
////////////////////////////////////////////////////////////////////////////////
