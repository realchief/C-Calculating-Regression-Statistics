#include "Common.h"
#include "StatisticalAnalysis/RegressionMask.h"
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
RegressionMask::RegressionMask( size_type predictor_count, bool enabled )
:   _interesting_mask   ( predictor_count, false ),
    _regression_mask    ( predictor_count, false ),
    _response_index     ( 0 ),
    _valid_mask         ( predictor_count, enabled )
{
    _regression_indices = indicesOf( _regression_mask );
}
////////////////////////////////////////////////////////////////////////////////
RegressionMask::RegressionMask()
:   _interesting_mask   (),
    _regression_mask    (),
    _response_index     (),
    _valid_mask         (),
	_regression_indices()
{
}
///////////////////////////////////////////////////////////////////////////

///
/// Destructor
///
RegressionMask::~RegressionMask( )
{
}
////////////////////////////////////////////////////////////////////////////////
/// Reference: https://www.quora.com/What-is-the-intuition-behind-the-masking-effects-on-linear-regression-in-multi-class-classification
/// Reference: https://stackoverflow.com/questions/21447595/how-to-mask-clouds-from-python-numpy-array-for-a-linear-regression-model
///////////////////////////////////////////////////////////////////////////////

void
RegressionMask::setInterestingPredictor( uint32_t index, bool on )
{
    DBC_REQUIRE( index < size() );

    _interesting_mask[index] = on && _valid_mask[index];
}
////////////////////////////////////////////////////////////////////////////////
void
RegressionMask::useResponseIndex( uint32_t index )
{
    _response_index = index;
}
////////////////////////////////////////////////////////////////////////////////
void
RegressionMask::setRegressionPredictor( uint32_t index, bool on )
{
    DBC_REQUIRE( index < size() );

    _regression_mask[index] = on && _valid_mask[index];

    _regression_indices = indicesOf( _regression_mask );
}
////////////////////////////////////////////////////////////////////////////////
void
RegressionMask::setValidPredictor( uint32_t index, bool on )
{
    DBC_REQUIRE( index < size() );

    _valid_mask[index] = on;
}
////////////////////////////////////////////////////////////////////////////////
void
RegressionMask::useAllPredictors( )
{
    _regression_mask    = _valid_mask;
    _interesting_mask   = _valid_mask;
    _regression_indices = indicesOf( _regression_mask );
}
///////////////////////////////////////////////////////////////////////////////

///
/// Build predictor index from the given predictor mask
/// Reference: http://anesthesiology.pubs.asahq.org/article.aspx?articleid=1945811
/// Reference: http://www.airwayworld.com/wp-content/uploads/2006/05/Incidence-and-Predictors-of-Difficult-and-Impossible-Mask-Ventilation.pdf
/// Reference: http://ieeexplore.ieee.org/document/7539284/?reload=true
///
PredictorIndices
RegressionMask::indicesOf( const PredictorMask& predictors ) const
{
    PredictorIndices indices;
 
    for( uint32_t i = 0, count = numeric_cast<uint32_t>(predictors.size()); 
        i < count; ++i )
    {
        if( predictors[i] )
        {
            indices.push_back( i );
        }
    }
    return indices;
}
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
