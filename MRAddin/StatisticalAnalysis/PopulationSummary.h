#ifndef MR_STATISTICAL_ANALYSIS_POPULATION_SUMMARY_H
#define MR_STATISTICAL_ANALYSIS_POPULATION_SUMMARY_H

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
using boost::uint32_t;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP PopulationSummary
{
public:
//  Lifetime:
    PopulationSummary( );
    PopulationSummary( uint32_t count, double sum, double sum_of_squares );
   ~PopulationSummary( );

//  USING COMPILER-GENERATED Copy constructor and assignment operator

//  Maffs:
    PopulationSummary&  operator+    ( const PopulationSummary& other);
    PopulationSummary&  operator-    ( const PopulationSummary& other);
    void                operator+=   ( const PopulationSummary& other);
    void                operator-=   ( const PopulationSummary& other);

//  Commands:
    void operator() ( double value ){add(value);}
    void add        ( double value );
    void remove     ( double value );
    void reset      ( );

//  Queries:
    uint32_t    count       ( ) const {return _count;}
    double      sum         ( ) const {return _sum;}
    double      sumOfSquares( ) const {return _sum_of_squares;}

//  Simple Stats:
    double      mean        ( ) const;
    double      stdDeviation( ) const;
    double      stdError    ( ) const;

//  (Group != Local) t and F:
    StatisticsResult    welch_t ( const PopulationSummary& total )  const;
    StatisticsResult    groupRelative_F ( const PopulationSummary& total )  const;

//  (Group == Local) t and F :
    StatisticsResult    zeroRelative_t  ( ) const;
    StatisticsResult    zeroRelative_F  ( ) const;

protected:

    uint32_t    _count;
    double      _sum;
    double      _sum_of_squares;
};
////////////////////////////////////////////////////////////////////////////////

    }   // namespace statistical_analysis
}   // namespace mr

////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_POPULATION_SUMMARY_H
////////////////////////////////////////////////////////////////////////////////
