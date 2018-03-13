#ifndef MR_F_T_SIGNIFICANCE_H
#define MR_F_T_SIGNIFICANCE_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

#include "StatisticalAnalysis/API.h"

///////////////////////////////////////////////////////////////////////////////
namespace mr
{
    namespace statistical_analysis
    {
        double STATISTICAL_ANALYSIS_EXPIMP FSig         (double f, double df_num, double df_den);
        double STATISTICAL_ANALYSIS_EXPIMP TSig         (double t, double df);
    
        double STATISTICAL_ANALYSIS_EXPIMP SigOfMeanDif (double mean_1, 
                             double std_dev_1, 
                             int count_1, 
                             double mean_2, 
                             double std_dev_2, 
                             int count_2);

        double STATISTICAL_ANALYSIS_EXPIMP SigOfMeanDif (double mean_1, 
                             double std_dev_1, 
                             int count_1, 
                             double mean_2);

		double STATISTICAL_ANALYSIS_EXPIMP SigOfStDevDif(double std_dev_1,
							int count_1,
							double std_dev_2,
							int count_2);

    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////
#endif // MR_F_T_SIGNIFICANCE_H
///////////////////////////////////////////////////////////////////////////////
