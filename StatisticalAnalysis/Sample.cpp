#include "Common.h"
#include "StatisticalAnalysis/Sample.h"
#include "StatisticalAnalysis/Episode.h"
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
Sample::Sample( sample_size_type num_observations_estimate )
{
    _observations.reserve( num_observations_estimate );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Constructor
///
Sample::Sample( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
Sample::~Sample( )
{
}
////////////////////////////////////////////////////////////////////////////////

///
/// Calculate and return the sample variance-covariance matrix
///
SymMatrix               
Sample::covarianceMatrix( ) const
{
    return (_means.size() == 0)
        ? covarianceMatrixImpl<false>() 
        : covarianceMatrixImpl<true>();
}
////////////////////////////////////////////////////////////////////////////////
ostream&
Sample::dump(ostream& stream) const
{
    forEach( _observations, boost::bind(
        &Sample::dumpObservation, this, 
            _1, boost::ref(stream) ) );

    return stream;
}
////////////////////////////////////////////////////////////////////////////////
void
Sample::dumpObservation(const Vector& observation, ostream& stream) const
{
    stream.precision(30);

    for( sample_size_type i = 0, last = observation.size(); i < last; ++i )
    {
        stream << observation(i) << '\t';
    }
    stream << endl;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
