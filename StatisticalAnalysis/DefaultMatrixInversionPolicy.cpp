#include "Common.h"
#include "StatisticalAnalysis/DefaultMatrixInversionPolicy.h"
#include "StatisticalAnalysis/GaussJordanInversion.h"

namespace mr
{
	namespace statistical_analysis
	{

Matrix
DefaultMatrixInversionPolicy::invert(const Matrix & m)
{
	 GaussJordanInversion<double> inverse;
	 try
	 {
		return inverse(m);
	 }
	 catch (GaussJordanInversionFailure &)
	 {
		 setColinearityDetected();
		 throw;
	 }
}

	}
}
