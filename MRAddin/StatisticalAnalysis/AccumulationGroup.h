#ifndef MR_STATISTICAL_ANALYSIS_ACCUMULATION_GROUP_H
#define MR_STATISTICAL_ANALYSIS_ACCUMULATION_GROUP_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

///////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/Accumulation.h"
#include "StatisticalAnalysis/Observation.h"
#include "StatisticalAnalysis/Response.h"
#include "StatisticalAnalysis/NumericContainersConfiguration.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "Infrastructure/MathUtils.h"
///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost;
///////////////////////////////////////////////////////////////////////////////

template <typename ContainersConfiguration>
class basic_accumulation_group
{
public:
	typedef boost::shared_ptr<basic_accumulation<ContainersConfiguration> > BasicAccumulationPtr;
//  Lifetime
                basic_accumulation_group   ( );
    explicit    basic_accumulation_group   ( uint32_t num_predictors );
	~basic_accumulation_group   ( ) {};

//  Commands:
    void add        ( BasicAccumulationPtr accumulation );
    void reset      ( );
    void operator*= ( double factor );

//  Commands
    void accumulate ( const basic_observation<ContainersConfiguration> & observation );

//  Navigable Associations
    const basic_accumulation<ContainersConfiguration> & accumulation( uint32_t a ) const;
    const basic_accumulation<ContainersConfiguration> & operator[]  ( uint32_t a ) const {return accumulation(a);}

//  Queries:
    uint32_t predictorCount     ( )             const {return _num_predictors;}
    uint64_t observationCount   ( uint32_t a )  const;

protected:
    typedef vector<BasicAccumulationPtr>    Accumulations;

    Accumulations   _accumulations;
    uint32_t        _num_predictors;
};
///////////////////////////////////////////////////////////////////////////////

///
/// Add an observation to the appropriate accumulation
///
template <typename ContainersConfiguration>
inline void 
basic_accumulation_group<ContainersConfiguration>::accumulate( const basic_observation<ContainersConfiguration> & observation )
{
    DBC_REQUIRE( observation.response().index() < _accumulations.size() );

    _accumulations[observation.response().index()]->accumulate(observation);
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return a reference to the specified accumulation
///
template <typename ContainersConfiguration>
inline const basic_accumulation<ContainersConfiguration> & 
basic_accumulation_group<ContainersConfiguration>::accumulation( uint32_t a ) const
{
    DBC_REQUIRE( a < _accumulations.size() );

    return *_accumulations[a];
}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline uint64_t 
basic_accumulation_group<ContainersConfiguration>::observationCount( uint32_t a ) const
{
    DBC_REQUIRE( a < _accumulations.size() );

    return _accumulations[a]->observations();
}

typedef basic_accumulation_group<DefaultContainers> AccumulationGroup;

///////////////////////////////////////////////////////////////////////////////
    }
}

#include "AccumulationGroup.inl"

///////////////////////////////////////////////////////////////////////////////
#endif //   MR_STATISTICAL_ANALYSIS_ACCUMULATION_GROUP_H
///////////////////////////////////////////////////////////////////////////////
