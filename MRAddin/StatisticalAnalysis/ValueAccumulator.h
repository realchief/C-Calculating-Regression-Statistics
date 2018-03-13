#ifndef MR_VALUE_ACCUMULATOR_H
#define MR_VALUE_ACCUMULATOR_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/StatisticalAnalysisAPI.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
        using boost::uint32_t;
        using boost::uint64_t;
        using std::vector;
////////////////////////////////////////////////////////////////////////////////

class STATISTICAL_ANALYSIS_EXPIMP ValueAccumulator
{
public:
    typedef boost::shared_ptr<double>   LazyDouble;
    typedef boost::shared_ptr<uint32_t> LazyCount;

    enum CounterIndex
    {
        COUNT_NAN_DOSET,
        COUNT_NAN_NA,
        COUNT_NAN_NOT_SET,
        COUNT_NAN_DIVBY0,
        COUNT_NAN_INVALID,
        COUNT_NAN_UNKNOWN,
        COUNT_VALUE_NEGATIVE,
        COUNT_VALUE_ZERO,
        COUNT_VALUE_POSITIVE,
        NUMBER_OF_COUNTERS
    };

//  Lifetime
    ValueAccumulator( );
   ~ValueAccumulator( );
    ValueAccumulator( const ValueAccumulator& source );
    void operator=  ( const ValueAccumulator& rhs );
    
//  Summary Base Queries:
    LazyDouble  anovaT              ( ) const {return _anova_t;}
    uint32_t    counter             ( uint32_t  index ) const;
    uint32_t    count               ( ) const {return _count_values;}
    LazyDouble  geometricMean       ( ) const {return _geometric_mean;}
    LazyDouble  huberMean           ( ) const {return _huber_mean;}
    double      maximumValue        ( ) const {return _maximum;}
    double      mean                ( ) const;
    double      meanPerStdDev       ( ) const;
    double      meanPerStdErr       ( ) const;
    LazyDouble  median              ( ) const {return _median;}
    double      minimumValue        ( ) const {return _minimum;}
    uint32_t    nanCount            ( ) const;  
    uint32_t    sampleCount         ( ) const;  // Does not include NANs
    double      standardDeviation   ( ) const;
    double      standardError       ( ) const;
    double      sum                 ( ) const {return _sum;}
    double      sumOfSquares        ( ) const {return _sum_of_squares;}
    LazyDouble  trimmedMean         ( ) const {return _trimmed_mean;}
    LazyCount   uniqueValueCount    ( ) const {return _unique_value_count;}
    uint32_t    valueCount          ( ) const;

//  Commands:
    void addValue           ( double value );
    void initialise         ( );
    void setOffsets         ( uint64_t  reference_offset, uint64_t  offset );

    void setGeometricMean   ( double value )    {_geometric_mean.reset      ( new double ( value ) );}
    void setHuberMean       ( double value )    {_huber_mean.reset          ( new double ( value ) );}
    void setMedian          ( double value )    {_median.reset              ( new double( value ) );}
    void setTrimmedMean     ( double value )    {_trimmed_mean.reset        ( new double( value ) );}
    void setUniqueValueCount( uint32_t  value ) {_unique_value_count.reset  ( new uint32_t ( value ) );}
    void setAnovaT          ( const ValueAccumulator& all_values );

//  Static Helpers:
    static bool     isValidVariance ( double std_deviation, double std_error, uint32_t  count );
    static double   stdDev          ( uint32_t count, double sum, double sum_of_squares );
    static double   stdErr          ( uint32_t count, double sum, double sum_of_squares );

protected:
    typedef vector<uint32_t>    Counters;
    typedef vector<double>      Values;

//  Commands:
    void    accumulateNaN   ( int nan_type );
    void    accumulateValue ( double value );

//  Summary data:
    LazyDouble  _anova_t;
    uint32_t    _count_values;
    Counters    _counters;
    LazyDouble  _geometric_mean;
    LazyDouble  _huber_mean;
    double      _maximum;
    LazyDouble  _median;
    double      _minimum;
    double      _sum;
    double      _sum_of_squares;
    LazyDouble  _trimmed_mean;
    LazyCount   _unique_value_count;
    Values      _values;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace dataset_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_VALUE_ACCUMULATOR_H
////////////////////////////////////////////////////////////////////////////////
