#include "StatisticalAnalysis/CovarianceMatrixGenerator.h" 
#include "StatisticalAnalysis/GaussJordanInversion.h"
#include "Infrastructure/ContainerAlgorithm.h"
#include "DesignByContract.h"
#include "DebugNew.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template <typename ContainersConfiguration>
inline basic_episode_partition<ContainersConfiguration>::basic_episode_partition( SimultaneousEpisodesPtr batch )
:   _key    ( batch->key() ),
    _digest ( batch->digest() )
{
    include ( batch );
}

///
/// Destructor
///
template <typename ContainersConfiguration>
inline
basic_episode_partition<ContainersConfiguration>::~basic_episode_partition( )
{
}
////////////////////////////////////////////////////////////////////////////////

template <typename ContainersConfiguration>
inline bool    
basic_episode_partition<ContainersConfiguration>::canInclude( SimultaneousEpisodesPtr episodes ) const 
{
	return _key == episodes->key();
}
////////////////////////////////////////////////////////////////////////////////

///
/// Add the batch of simultaneous observations
///
template <typename ContainersConfiguration>
inline void 
basic_episode_partition<ContainersConfiguration>::include( SimultaneousEpisodesPtr batch )
{
    _batches.fast_insert( batch );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the inverse covariance matrix
///
template <typename ContainersConfiguration>
inline SymMatrix 
basic_episode_partition<ContainersConfiguration>::covarianceInverse( ) const
{
//  Early-out if fewer "days" than "markets"
    if( _batches.size() <= _digest.size() )
    {
//      Just use the identity matrix in this partition
        return IdentityMatrix( _digest.size() );
    }

    CovarianceMatrixGenerator<false> covariance( _digest.size() );

    forEach( _batches, boost::bind( 
        &CovarianceMatrixGenerator<false>::include, 
            &covariance, boost::bind(
                &SimultaneousEpisodes::localResiduals, _1) ) );

    GaussJordanInversion<double> inverse;

    return covariance.validSample() 
        ?   inverse(covariance())
        :   IdentityMatrix(_digest.size());
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
