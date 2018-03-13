#ifndef MR_STATISTICAL_ANALYSIS_SYNOPSIS_H
#define MR_STATISTICAL_ANALYSIS_SYNOPSIS_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "Infrastructure/MathUtils.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
using namespace std;
////////////////////////////////////////////////////////////////////////////////

class STATISTICAL_ANALYSIS_EXPIMP Synopsis
{
    typedef vector<uint32_t >  Counters;

public:
    typedef boost::shared_ptr<double>   Statistic;
    typedef boost::shared_ptr<uint32_t> UniqueValueCount;
    //typedef std::pair<double, double>   Range;

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
    Synopsis        ( );
   ~Synopsis        ( );
    Synopsis        ( const Synopsis& source );
    void operator=  ( const Synopsis& rhs );
    
//  Queries:
    Statistic           grpRel_t            ( ) const {return _grp_rel_t;}
    uint32_t            counter             ( uint32_t  index ) const;
    uint32_t            count               ( ) const {return boost::numeric_cast<uint32_t>(_values.size());}
    uint32_t            countNegative       ( ) const {return _counters[COUNT_VALUE_NEGATIVE];}
    uint32_t            countPositive       ( ) const {return _counters[COUNT_VALUE_POSITIVE];}
    uint32_t            countZero           ( ) const {return _counters[COUNT_VALUE_ZERO];}
    double              maximum             ( ) const {return _maximum;}
    Statistic           mean                ( ) const {return _mean;}
    double              meanPerStdDev       ( ) const;
    double              meanPerStdErr       ( ) const;
    double              stdErrPerStdDev     ( ) const;
    double              stdDevPerStdErr     ( ) const;
    Statistic           median              ( ) const {return _median;}
    double              minimum             ( ) const {return _minimum;}
    uint32_t            nanCount            ( ) const;  
    double              standardDeviation   ( ) const;
    double              standardError       ( ) const;
    double              sum                 ( ) const {return _sum;}
    double              sumAbs              ( ) const {return _sum_abs;}
    double              sumOfSquares        ( ) const {return _sum_squares;}
    double              meanAbs             ( ) const;
    double              meanSquares         ( ) const;
    UniqueValueCount    uniqueValueCount    ( ) const {return _unique_value_count;}
    //PairDouble          range               ( ) const {return std::make_pair<double, double>(_minimum, _maximum);}
    double              pctPositive         ( ) const;
    double              pctNegative         ( ) const;
    double              pctZero             ( ) const;
    double              pctNaN              ( ) const;
    double              pctNonPositive      ( ) const;
    double              pctNonNegative      ( ) const;
    double              pctNonZero          ( ) const;
    double              pctNonNaN           ( ) const;
    std::string         formatted           ( const std::string& sep );

    double Mean() const
    {
        return (count() > 0) ? sum()/count() : infrastructure::math::NAN_DIVBY0;
    }

    double StDev() const
    {
        return standardDeviation();
    }


//  Commands:
    void    addValue                    ( double value );
    void    calculateGrpRel_t           ( );
    void    calculateHuberMean          ( );
    void    calculateKurtosis           ( );
    void    calculateMean               ( );
    void    calculateMedian             ( );
    void    calculateSkewness           ( );
    void    calculateTrimmedMean        ( );
    void    calculateUniqueValueCount   ( );
    void    initialise                  ( );

protected:

//  Sorting:
    void    sort    ( );
    bool    sorted  ( ) const {return _sorted;}

//  Commands:
    void accumulateNaN      ( int nan_type );
    void accumulateValue    ( double value );
    void setMean            ( double value )                {_mean.reset                ( new double( value ) );}
    void setMedian          ( double value )                {_median.reset              ( new double( value ) );}
    void setUniqueValueCount( uint32_t value )              {_unique_value_count.reset  ( new uint32_t( value ) );}
    void setGrpRel_t        ( const Synopsis& population );

//  Summary data:
    Statistic           _grp_rel_t;
    Counters            _counters;
    double              _maximum;
    Statistic           _mean;
    Statistic           _median;
    double              _minimum;
    bool                _sorted;
    double              _sum;
    double              _sum_abs;
    double              _sum_squares;
    UniqueValueCount    _unique_value_count;
    std::vector<double> _values;
};
////////////////////////////////////////////////////////////////////////////////
inline double 
Synopsis::pctPositive( ) const
{
    return 100.0 * _counters[COUNT_VALUE_POSITIVE] / count();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctNegative( ) const
{
    return 100.0 * _counters[COUNT_VALUE_NEGATIVE] / count();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctZero( ) const
{
    return 100.0 * _counters[COUNT_VALUE_ZERO] / count();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctNaN( ) const
{
    return 100.0 * nanCount()/(nanCount() + count());
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctNonPositive( ) const
{
    return 100.0 - pctPositive();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctNonNegative( ) const
{
    return 100.0 - pctNegative();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctNonZero( ) const
{
    return 100.0 - pctZero();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::pctNonNaN( ) const
{
    return 100.0 - pctNaN();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::stdDevPerStdErr( ) const
{
    return standardDeviation()/standardError();
}
////////////////////////////////////////////////////////////////////////////////
inline double
Synopsis::stdErrPerStdDev( ) const
{
    return standardError()/standardDeviation();
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_SYNOPSIS_H
////////////////////////////////////////////////////////////////////////////////
