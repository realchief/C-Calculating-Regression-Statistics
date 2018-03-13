#ifndef MR_STATISTICAL_ANALYSIS_SAMPLE_H
#define MR_STATISTICAL_ANALYSIS_SAMPLE_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
#include "StatisticalAnalysis/CovarianceMatrixGenerator.h"
#include "Infrastructure/ContainerAlgorithm.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace infrastructure;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP Sample
{
public:
    typedef vector<Vector>          Observations;
    typedef Observations::size_type sample_size_type;
    typedef Vector::size_type       observation_size_type;

public:
//  Lifetime:
    explicit    Sample( sample_size_type num_observations_estimate );
                Sample( );
               ~Sample( );

//  Navigable Associations:
    const Observations& operator()  ( ) const   {return _observations;}
    Observations&       operator()  ( )         {return _observations;}
    const Vector&       means       ( ) const   {return _means;}
    Vector&             means       ( )         {return _means;}

//  Commands:

//  Queries:
    sample_size_type        observationCount( ) const;
    observation_size_type   variateCount    ( ) const;
    SymMatrix               covarianceMatrix( ) const;

//  Diagnostics/Test
    ostream&    dump            ( ostream& stream ) const;

    void        dumpObservation ( const Vector& observation, 
                                  ostream& stream) const;

private:
//  Associations:
    Observations    _observations;
    Vector          _means;

    template<bool with_means>
    SymMatrix covarianceMatrixImpl( ) const;

//  Not Implemented
    Sample          ( const Sample& );
    void operator=  ( const Sample& );
};
////////////////////////////////////////////////////////////////////////////////
inline Sample::sample_size_type
Sample::observationCount( ) const
{
    return _observations.size();
}
////////////////////////////////////////////////////////////////////////////////
inline Sample::observation_size_type
Sample::variateCount( ) const
{
    return _observations.empty() ? 0 : _observations.begin()->size();
}
////////////////////////////////////////////////////////////////////////////////
template< > inline SymMatrix 
Sample::covarianceMatrixImpl<false>( ) const
{
    CovarianceMatrixGenerator<false> covariance(variateCount());

    forEach(_observations, boost::bind(
        &CovarianceMatrixGenerator<false>::include, 
            &covariance, _1 ) );

    return covariance();
}
////////////////////////////////////////////////////////////////////////////////
template< > inline SymMatrix 
Sample::covarianceMatrixImpl<true>( ) const
{
    CovarianceMatrixGenerator<true> covariance(means());

    forEach(_observations, boost::bind(
        &CovarianceMatrixGenerator<true>::include, 
            &covariance, _1 ) );

    return covariance();
}
//////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
