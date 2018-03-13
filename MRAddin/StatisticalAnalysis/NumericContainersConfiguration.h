#ifndef MR_NUMERIC_CONTAINERS_CONFIGURATION_H_INCLUDED
#define MR_NUMERIC_CONTAINERS_CONFIGURATION_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

#include "StatisticalAnalysis/MatrixDefinitions.h"

namespace mr
{
	namespace statistical_analysis
	{
		struct DefaultContainers
		{
			typedef Vector vector_type;
			typedef Matrix matrix_type;
		};
	}
}


#endif //MR_NUMERIC_CONTAINERS_CONFIGURATION_H_INCLUDED
