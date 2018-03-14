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
/// Reference: http://blog.minitab.com/blog/adventures-in-statistics-2/how-to-interpret-regression-analysis-results-p-values-and-coefficients
/// Reference: https://statistics.laerd.com/stata-tutorials/linear-regression-using-stata.php
/// Reference: https://dss.princeton.edu/online_help/analysis/interpreting_regression.htm
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
/// Reference: http://blog.minitab.com/blog/adventures-in-statistics-2/how-to-interpret-regression-analysis-results-p-values-and-coefficients
/// Reference: https://statistics.laerd.com/stata-tutorials/linear-regression-using-stata.php
/// Reference: https://dss.princeton.edu/online_help/analysis/interpreting_regression.htm

void
RegressionResultBinder::appendResults( const RegressionMask& mask )
{
    _results.push_back(_regression->calculateResult(mask));
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
