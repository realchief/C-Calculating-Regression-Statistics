#ifndef MR_STATISTICAL_ANALYSIS_MATRIX_INVERSION_CANCELED_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_MATRIX_INVERSION_CANCELED_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

namespace mr
{
	namespace statistical_analysis
	{
		class MatrixInversionCanceled : public std::runtime_error
		{
		public:
			MatrixInversionCanceled() : std::runtime_error("Matrix inversion has been canceled.") {}
		};
	}
}

#endif
