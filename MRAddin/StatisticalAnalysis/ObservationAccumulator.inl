#include "DesignByContract.h"
#include "DebugNew.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline basic_observation_accumulator<ContainersConfiguration>::basic_observation_accumulator( uint32_t num_predictors )
:   _accumulations  ( num_predictors ),
    _auto_ema       ( true ),
    _ema_weight     ( 1.0 ),
    _discarded      ( 0 )
{
}
/////////////////////////////////////////////////////////////////////////////////
//template <typename ContainersConfiguration>
//inline void
//basic_observation_accumulator<ContainersConfiguration>::Reset( )
//{
//    _auto_ema = true;
//    _ema_weight = 1.0;
//    _accumulations.reset();
//}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
basic_observation_accumulator<ContainersConfiguration>::setAutoEMA(const bool auto_ema)
{
    _auto_ema = auto_ema;                                                 
}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
basic_observation_accumulator<ContainersConfiguration>::setEMABoundary()
{
    _accumulations *= _ema_weight;
}
///////////////////////////////////////////////////////////////////////////////
template <typename ContainersConfiguration>
inline void 
basic_observation_accumulator<ContainersConfiguration>::setEMALength(const double ema_length)
{
    if (ema_length > 0)
        _ema_weight = pow(0.15, 1.0 / ema_length);
    else if (ema_length < 0)
        _ema_weight = 1.0;
    else
        _ema_weight = 0.0;
}
///////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analyis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////