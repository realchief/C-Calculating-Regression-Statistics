#include "Infrastructure/ContainerAlgorithm.h"
#include "Infrastructure/MathUtils.h"
#include "Infrastructure/Deleter.h"
#include "DesignByContract.h"
////////////////////////////////////////////////////////////////////////////////

namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
using namespace infrastructure::math;
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
template<typename ContainersConfiguration>
inline basic_simultaneous_episodes<ContainersConfiguration>::basic_simultaneous_episodes( basic_episode<ContainersConfiguration> * episode)
:   _episodes(),
	_digest(),
	_key(),
	_when( episode->when() )
{
    include( episode );
}

template<typename ContainersConfiguration>
inline basic_simultaneous_episodes<ContainersConfiguration>::basic_simultaneous_episodes(const basic_simultaneous_episodes & other) :
	_episodes(other._episodes),
	_digest(other._digest),
	_key(other._key),
	_when(other._when)
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
template<typename ContainersConfiguration>
inline basic_simultaneous_episodes<ContainersConfiguration>::~basic_simultaneous_episodes( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Add the episode, and update the digest
///
template<typename ContainersConfiguration>
inline void 
basic_simultaneous_episodes<ContainersConfiguration>::include( basic_episode<ContainersConfiguration> * episode )
{
	if (episode->isValid())
	{
		_episodes.fast_insert( episode );
		_digest.fast_insert( episode->what() );
	}
}
////////////////////////////////////////////////////////////////////////////////

///
/// Build the key using the contents of the universal partition
/// By collecting the difference between the universal digest and this 
/// digest, we can keep the partition comparisons short.
///
template<typename ContainersConfiguration>
inline void
basic_simultaneous_episodes<ContainersConfiguration>::buildKey( const Digest& universal_digest )
{
	DBC_REQUIRE(universal_digest.size() >= _digest.size());

    _key.clear();

    _key.reserve( universal_digest.size() - _digest.size() );

    std::set_difference( 
        universal_digest.begin(), universal_digest.end(),
            _digest.begin(), _digest.end(),
                back_inserter( _key ) );

    std::sort( _key.begin(), _key.end() );

    forEach( _episodes, boost::bind(
        &Episode::updateIndex, _1, 
            boost::cref(universal_digest) ));
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the residual for each of the episodes
///
template<typename ContainersConfiguration>
inline Vector   
basic_simultaneous_episodes<ContainersConfiguration>::localResiduals( ) const
{
    Vector local_residuals;

    local_residuals.resize( _episodes.size() );

    std::transform( _episodes.begin(), _episodes.end(), 
        local_residuals.begin(), boost::bind( &Episode::residualValue, _1 ) );

    return local_residuals;
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate the residual for each of the episodes
///
template<typename ContainersConfiguration>
inline Vector   
basic_simultaneous_episodes<ContainersConfiguration>::residuals( const Digest& digest ) const
{
    Vector residuals( digest.size() );

    for (unsigned i = 0; i < residuals.size (); ++ i)
        residuals (i) = NAN_NOT_SET;

    for( Episodes::const_iterator episode = _episodes.begin(), 
        last = _episodes.end(); episode != last; ++episode )
    {
        const Episode& e = **episode;

        residuals(e.index()) = e.residualValue();
    }

    return residuals;
}

template<typename ContainersConfiguration>
inline void
basic_simultaneous_episodes<ContainersConfiguration>::removeMarkets(const lazy_set<int32_t> & markets_to_remove)
{//markets are ordered in the same way as their ids in the lazy set

	Episodes::iterator Iepisodes = _episodes.begin();
	for (lazy_set<int32_t>::const_iterator I = markets_to_remove.begin(), Iend = markets_to_remove.end(); I != Iend; ++I)
	{
		while (Iepisodes != _episodes.end() &&
			(*Iepisodes)->what() < (*I))
			++Iepisodes;

		if (Iepisodes == _episodes.end())
		{
			break;
		}

		while (Iepisodes != _episodes.end() && (*Iepisodes)->what() == (*I))
		{
			Iepisodes = _episodes.erase(Iepisodes);
		}
	}

	Digest new_digest;
	
	std::set_difference(_digest.begin(), _digest.end(), markets_to_remove.begin(), markets_to_remove.end(),
		std::inserter(new_digest, new_digest.begin()),
		std::less<double>());
	
	_digest = new_digest;

	DBC_ALWAYS_CHECK(_digest.size() == _episodes.size());
}

////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
