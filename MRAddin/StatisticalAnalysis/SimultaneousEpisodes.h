#ifndef MR_GLS_OBSERVATION_BATCH_H
#define MR_GLS_OBSERVATION_BATCH_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/Episode.h"
#include "StatisticalAnalysis/SortPredicates.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace infrastructure;
////////////////////////////////////////////////////////////////////////////////
template<typename ContainersConfiguration>
class basic_simultaneous_episodes
{
	typedef basic_simultaneous_episodes<ContainersConfiguration> MyType;
protected:
    typedef vector< double >    Key;

public:
	typedef lazy_set< basic_episode<ContainersConfiguration> *, SimultaneousPred<basic_episode<ContainersConfiguration>* > > Episodes;
	typedef boost::shared_ptr<MyType> SimultaneousEpisodesPtr;

//  Lifetime:
    explicit    basic_simultaneous_episodes( basic_episode<ContainersConfiguration> * episode);
	basic_simultaneous_episodes(const basic_simultaneous_episodes & other);

    ~basic_simultaneous_episodes( );

//  Navigable Associations:
    const Episodes& operator()  ( ) const {return _episodes;}
	const Key&      key         ( ) const {return _key;}
    const Digest&   digest      ( ) const {return _digest;}

//  Commands:
    void include    ( basic_episode<ContainersConfiguration> * episode );
    void buildKey   ( const Digest& universal_digest );
	void removeMarkets(const lazy_set<int32_t> & markets_to_remove);

//  Residuals Access
    Vector  localResiduals  ( )                         const;
    Vector  residuals       ( const Digest& digest )    const;

//  Simple Queries:
    bool    canInclude      ( const basic_episode<ContainersConfiguration> * episode )        const { return _when == episode->when();}
    double  when            ( )                         const { return _when;}
	size_t  count			( )							const { return _episodes.size(); }

private:
//  Associations:
    Episodes    _episodes;
    Digest      _digest;
    Key         _key;

//  Attributes:
    double  _when;
};

typedef basic_simultaneous_episodes<DefaultContainers> SimultaneousEpisodes;

////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr

#include "SimultaneousEpisodes.inl"

////////////////////////////////////////////////////////////////////////////////
#endif // MR_GLS_OBSERVATION_BATCH_H
////////////////////////////////////////////////////////////////////////////////
