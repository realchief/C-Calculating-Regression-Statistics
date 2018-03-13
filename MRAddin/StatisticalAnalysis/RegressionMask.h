#ifndef MR_STATISTICAL_ANALYSIS_REGRESSION_MASK_H
#define MR_STATISTICAL_ANALYSIS_REGRESSION_MASK_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
#include "StatisticalAnalysis/API.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
class STATISTICAL_ANALYSIS_EXPIMP RegressionMask
{
public:
    typedef PredictorMask::const_iterator   const_iterator;
    typedef uint32_t                        size_type;

//  Lifetime:
    RegressionMask  ( size_type predictor_count, bool enabled );
	RegressionMask  ();
   ~RegressionMask  ( );

//  Navigable associations:
    const PredictorIndices& indices     ( ) const {return _regression_indices;}
    const PredictorMask&    operator()  ( ) const {return _regression_mask;}
    const PredictorMask&    interesting ( ) const {return _interesting_mask;}

//  Commands:
    void setInterestingPredictor( uint32_t index, bool on );
    void setRegressionPredictor ( uint32_t index, bool on );
    void setValidPredictor      ( uint32_t index, bool on );
    void useAllPredictors       ( );
    void useResponseIndex       ( uint32_t index );

//  Queries:
    bool        usesAllPredictors   ( ) const {return _regression_indices.size() == _regression_mask.size();}
    uint32_t    responseIndex       ( ) const {return _response_index;}

//  STL-like interface
    const_iterator  begin   ( ) const {return _regression_mask.begin();}
    const_iterator  end     ( ) const {return _regression_mask.end();}
    size_type       size    ( ) const {return numeric_cast<size_type>(_regression_mask.size());}

private:
//  Queries:
    PredictorIndices    indicesOf( const PredictorMask& predictors ) const;

//  Implementation
    PredictorMask       _interesting_mask;      // Predictors that are interesting for display purposes
    PredictorIndices    _regression_indices;    // Indices of predictors used by the regression calculation
    PredictorMask       _regression_mask;       // Predictors used by the regression calculation
    uint32_t            _response_index;        // The index into the observation accumulation
    PredictorMask       _valid_mask;            // Predictors that have valid observation set
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_REGRESSION_MASK_H
////////////////////////////////////////////////////////////////////////////////
