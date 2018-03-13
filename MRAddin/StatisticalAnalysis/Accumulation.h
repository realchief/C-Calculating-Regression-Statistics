#ifndef MR_STATISTICAL_ANALYSIS_ACCUMULATION_H
#define MR_STATISTICAL_ANALYSIS_ACCUMULATION_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/Observation.h"
#include "StatisticalAnalysis/NumericContainersConfiguration.h"
#include "StatisticalAnalysis/Variance.h"
#include "Infrastructure/MathUtils.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace boost;
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
class basic_accumulation
{
public:
//  Lifetime
    virtual ~basic_accumulation   ( ){}

	typedef boost::shared_ptr<basic_accumulation<ContainersConfiguration> > AccumulationPtr;

//  Abstract Commands:
    virtual void accumulate ( const basic_observation<ContainersConfiguration> & observation )  = 0;

//  Commands
    virtual void operator*= ( double factor );
    virtual void reset      ( );

//  Navigable Associations
	const SymMatrix &   sumXX ( ) const {return _sum_xx;}
    const Vector &      sumXY ( ) const {return _sum_xy;}

//  Queries:
    uint64_t    observations( ) const {return _observations;}
    uint32_t    predictors  ( ) const {return _predictors;}
    double      sumWeights  ( ) const {return _sum_weights;}
    double      sumY        ( ) const {return _sum_y;}
    double      sumYY       ( ) const {return _sum_yy;}

//  Predictor and Response Statistics:
    double predictorMean    ( uint32_t p ) const;
    double predictorStdDev  ( uint32_t p ) const;
    double responseMean     ( ) const;
    double responseStdDev   ( ) const;

protected:
//  Lifetime
                basic_accumulation( );
    explicit    basic_accumulation( uint32_t predictor_count );

//  Implementation:
    Vector _sum_x;
    Vector _sum_xy;
	SymMatrix _sum_xx;

//  Attributes
    uint32_t _predictors;
    uint64_t _observations;
    double  _sum_weights;
    double  _sum_y;
    double  _sum_yy;
};
///////////////////////////////////////////////////////////////////////////////

typedef basic_accumulation<DefaultContainers> Accumulation;

    }
}

#include "Accumulation.inl"

///////////////////////////////////////////////////////////////////////////////
#endif //   MR_STATISTICAL_ANALYSIS_ACCUMULATION_H
///////////////////////////////////////////////////////////////////////////////
