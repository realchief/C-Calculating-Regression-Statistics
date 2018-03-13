

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
using namespace infrastructure;
using namespace infrastructure::math;
using namespace boost;
///////////////////////////////////////////////////////////////////////////////

///
/// Default Constructor
///
template <typename ContainersConfiguration>
inline basic_accumulation_group<ContainersConfiguration>::basic_accumulation_group(  )
:   _num_predictors ( 0 )
{
}
///////////////////////////////////////////////////////////////////////////////

///
/// Initialising Constructor
///
template <typename ContainersConfiguration>
inline basic_accumulation_group<ContainersConfiguration>::basic_accumulation_group( uint32_t num_predictors )
:   _num_predictors ( num_predictors )
{
}
///////////////////////////////////////////////////////////////////////////////

///
/// Add an accumulation to the group Constructor
///
template <typename ContainersConfiguration>
inline void
basic_accumulation_group<ContainersConfiguration>::add( typename basic_accumulation_group<ContainersConfiguration>::BasicAccumulationPtr accumulation )
{
    _accumulations.push_back( accumulation );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Reset the accumulations
///
template <typename ContainersConfiguration>
inline void 
basic_accumulation_group<ContainersConfiguration>::reset( )
{
	infrastructure::forEach( _accumulations, boost::bind( &basic_accumulation<ContainersConfiguration>::reset, _1 ) );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Multiply the accumulations by the factor
///
template <typename ContainersConfiguration>
inline void 
basic_accumulation_group<ContainersConfiguration>::operator*=( double factor )
{
    infrastructure::forEach( _accumulations, boost::bind( &basic_accumulation<ContainersConfiguration>::operator*=, _1, factor ) );
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
