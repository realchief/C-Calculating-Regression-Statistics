#ifndef MR_STATISTICAL_ANALYSIS_LAGGING_OPTIONS_H
#define MR_STATISTICAL_ANALYSIS_LAGGING_OPTIONS_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
class LaggingOptions
{
public:
    enum SampleConstraints    { NO_CONSTRAINTS, 
                                USE_ONLY_COMMON_RESPONSES, 
                                USE_ONLY_COMMON_PREDICTORS};

//  Lifetime:
    LaggingOptions( );
   ~LaggingOptions( ){}

//  Navigable associations:
    const Lags& lags( ) const   {return _lags;}
    Lags&       lags( )         {return _lags;}

//  Commands:
    void setLagByRecords    ( bool on );
    void setLags            ( const string& specification );
    void useAllObservations ( );
    void useCommonPredictors( );
    void useCommonResponses ( );

//  Queries:
    bool                lagByRecords        ( ) const   {return _lag_by_records;}
    bool                onlyCommonPredictors( ) const   {return _sample_constraints == USE_ONLY_COMMON_PREDICTORS;}
    bool                onlyCommonResponses ( ) const   {return _sample_constraints == USE_ONLY_COMMON_RESPONSES;}
    bool                sampleConstrained   ( ) const   {return _sample_constraints != NO_CONSTRAINTS;}
    SampleConstraints   sampleConstraints   ( ) const   {return _sample_constraints;}
    string              specification       ( ) const   {return _lag_specification;}

private:
    Lags                _lags;
    string              _lag_specification;
    bool                _lag_by_records;
    SampleConstraints   _sample_constraints;
};
////////////////////////////////////////////////////////////////////////////////
inline
LaggingOptions::LaggingOptions( )
:   _lag_specification  ( "0,1..10" ),
    _lag_by_records     ( true ),
    _sample_constraints ( NO_CONSTRAINTS )
{
    for( boost::int32_t i = 0; i <= 10; ++i )
    {
        _lags.insert( i );
    }
}
////////////////////////////////////////////////////////////////////////////////
inline void
LaggingOptions::setLagByRecords( bool on )
{
    _lag_by_records = on;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
LaggingOptions::setLags( const string& specification )
{
    _lag_specification  = specification;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
LaggingOptions::useAllObservations( )
{
    _sample_constraints = NO_CONSTRAINTS;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
LaggingOptions::useCommonPredictors( )
{
    _sample_constraints = USE_ONLY_COMMON_PREDICTORS;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
LaggingOptions::useCommonResponses( )
{
    _sample_constraints = USE_ONLY_COMMON_RESPONSES;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_LAGGING_OPTIONS_H
////////////////////////////////////////////////////////////////////////////////
