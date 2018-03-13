#include "Common.h"
#include "StatisticalAnalysis/RegressionResultBinder.h"
#include "StatisticalAnalysis/LinearRegression.h"
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
RegressionResultBinder::RegressionResultBinder( LinearRegressionPtr regression,
                                                bool multi_variate )
:   _regression     ( regression ),
    _multi_variate  ( multi_variate )
{
    DBC_REQUIRE( isNotNull(regression) );
}
////////////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
RegressionResultBinder::~RegressionResultBinder( )
{
}
////////////////////////////////////////////////////////////////////////////////
void
RegressionResultBinder::appendResults( const RegressionMask& mask )
{
    _results.push_back(_regression->calculateResult(mask));
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
