#ifndef MR_STATISTICAL_ANALYSIS_MODELLING_OPTIONS_H
#define MR_STATISTICAL_ANALYSIS_MODELLING_OPTIONS_H

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
class ModellingOptions
{
    typedef std::set<string> SortedNames;

public:
    enum    Multiplicity  { SINGLE_FACTOR, 
                            MULTI_FACTOR};

//  Lifetime:
  /*explicit*/  ModellingOptions( );
               ~ModellingOptions( );

//  Commands:
    void addFactor                  ( const std::string& factor );
    void produceSingleFactorModels  ( );
    void produceMultiFactorModels   ( );
    void removeFactor               ( const std::string& factor );
    void setFactorNames             ( const SortedNames& factor_names );
    void setResponseNormalizerName  ( const std::string& name );
    void setSwitchName              ( const std::string& name );
    void setRiskAversionAlpha       ( double risk_aversion_alpha ){_risk_aversion_alpha = risk_aversion_alpha;}

//  Queries:
    SortedNames     factorNames             ( ) const   {return _factor_names;}
    bool            isMultiFactor           ( ) const   {return _multiplicity == MULTI_FACTOR;}
    bool            isSingleFactor          ( ) const   {return _multiplicity == SINGLE_FACTOR;}
    Multiplicity    multiplicity            ( ) const   {return _multiplicity;}
    std::string     responseNormalizerName  ( ) const   {return _response_normalizer_name;}
    std::string     switchName              ( ) const   {return _switch_name;}
    double          riskAversionAlpha       ( ) const   {return _risk_aversion_alpha;}

protected:

    SortedNames     _factor_names;
    Multiplicity    _multiplicity;
    std::string     _response_normalizer_name;
    std::string     _switch_name;
    double          _risk_aversion_alpha;
};
////////////////////////////////////////////////////////////////////////////////
inline 
ModellingOptions::ModellingOptions( )
:   _multiplicity       ( SINGLE_FACTOR ),
    _risk_aversion_alpha( -2 )
{
}
////////////////////////////////////////////////////////////////////////////////
inline 
ModellingOptions::~ModellingOptions( )
{
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::addFactor( const std::string& factor )
{
    _factor_names.insert( factor );
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::produceSingleFactorModels( )
{
    _multiplicity = SINGLE_FACTOR;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::produceMultiFactorModels( )
{
    _multiplicity = MULTI_FACTOR;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::removeFactor( const std::string& factor )
{
    _factor_names.erase( factor );
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::setFactorNames( const SortedNames& factor_names )
{
    _factor_names = factor_names;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::setResponseNormalizerName( const std::string& name )
{
    _response_normalizer_name = name;
}
////////////////////////////////////////////////////////////////////////////////
inline void 
ModellingOptions::setSwitchName( const std::string& name )
{
    _switch_name = name;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_MODELLING_OPTIONS_H
////////////////////////////////////////////////////////////////////////////////
