#ifndef MR_STATISTICAL_ANALYSIS_DEFAULT_MATRIX_INVERSION_POLICY_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_DEFAULT_MATRIX_INVERSION_POLICY_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

#include "StatisticalAnalysis/API.h"
#include "MatrixInversionPolicy.h"

namespace mr
{
	namespace statistical_analysis
	{

class STATISTICAL_ANALYSIS_EXPIMP DefaultMatrixInversionPolicy : public MatrixInversionPolicy
{
public:
	DefaultMatrixInversionPolicy() : MatrixInversionPolicy() {}
	virtual Matrix invert(const Matrix & m);
};

	}
}


#endif //MR_STATISTICAL_ANALYSIS_DEFAULT_MATRIX_INVERSION_POLICY_H_INCLUDED
