#ifndef MR_STATISTICAL_ANALYSIS_API_H
#define MR_STATISTICAL_ANALYSIS_API_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef STATISTICAL_ANALYSIS_EXPORTS
#define STATISTICAL_ANALYSIS_EXPIMP __declspec(dllexport)
#else
#define STATISTICAL_ANALYSIS_EXPIMP __declspec(dllimport)
#endif

#ifdef _MSC_VER
#pragma warning( push, 1 )
#pragma warning( disable : 4244 )
#endif // ifdef _MSC_VER
////////////////////////////////////////////////////////////////////////////////
#include "Infrastructure/LazySet.h"
#include "SortPredicates.h"
////////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
class ANOVA_F;
class ANOVA_t;
class ConserveMeanImputer;
class CorrelationResult;
class ModellingOptions;
class LaggingOptions;
class Normalize;
class NormalizeNull;
class NormalizeIntercept;
class NormalizePredictor;
class NormalizeResponse;
class RegressionMask;
class RegressionResult;
class RegressionResultBinder;
class PredictedValueFunctor;
class Response;
class Sample;
class Synopsis;
class StatisticsResult;
class PopulationSummary;
////////////////////////////////////////////////////////////////////////////////
template <bool by_records> class LagAccumulator;
////////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<ANOVA_F>                  ANOVA_FPtr;
typedef boost::shared_ptr<ANOVA_t>                  ANOVA_tPtr;
typedef boost::shared_ptr<CorrelationResult>        CorrelationResultPtr;
typedef boost::shared_ptr<Normalize>                Normalizer;
typedef boost::shared_ptr<RegressionResult>         RegressionResultPtr;
typedef boost::shared_ptr<const RegressionResult>   RegressionResultCPtr;
typedef boost::shared_ptr<RegressionResultBinder>   RegressionBinderPtr;
typedef boost::shared_ptr<Sample>                   SamplePtr;
typedef boost::shared_ptr<Synopsis>                 SynopsisPtr;
typedef boost::shared_ptr<PopulationSummary>		PopulationSummaryPtr;
////////////////////////////////////////////////////////////////////////////////
typedef std::vector<double>                 RawResults;
typedef std::vector<double>                 Values;
typedef std::vector<StatisticsResult>       StatisticsResults;
typedef std::vector<RegressionResultPtr>    RegressionResults;
typedef std::vector<RegressionResultPtr>	RegressionResultVector;
typedef std::vector<bool>                   PredictorMask;
typedef std::vector<boost::uint32_t>        PredictorIndices;
typedef std::set<boost::int32_t>            Lags;
typedef infrastructure::lazy_set<double>    Digest;

struct DefaultContainers ;
template <typename ContainersConfiguration>
class basic_linear_regression ;
typedef basic_linear_regression<DefaultContainers> LinearRegression;
////////////////////////////////////////////////////////////////////////////////
    } // namespace statistical_analysis
} // namespace mr
////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma warning(pop)
#endif
////////////////////////////////////////////////////////////////////////////////
#endif  // MR_STATISTICAL_ANALYSIS_API_H
////////////////////////////////////////////////////////////////////////////////

