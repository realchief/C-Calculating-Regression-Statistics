#include "Common.h"
#include "StatisticalAnalysis/DefaultMatrixInversionPolicy.h"
#include "StatisticalAnalysis/GaussJordanInversion.h"

namespace mr
{
	namespace statistical_analysis
	{


/// Matrix
/// Reference: https://ocw.mit.edu/courses/mechanical-engineering/2-086-numerical-computation-for-mechanical-engineers-fall-2012/readings/MIT2_086F12_notes_unit3.pdf
/// Reference: http://faculty.cas.usf.edu/mbrannick/regression/regma.htm
/// Reference: http://keisan.casio.com/exec/system/14059932105271

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
