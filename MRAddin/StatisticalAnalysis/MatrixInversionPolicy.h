#ifndef MR_STATISTICAL_ANALYSIS_MATRIX_INVERTER_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_MATRIX_INVERTER_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

#include "StatisticalAnalysis/API.h"
#include "StatisticalAnalysis/MatrixDefinitions.h"

namespace mr
{
	namespace statistical_analysis
	{

class STATISTICAL_ANALYSIS_EXPIMP MatrixInversionPolicy
{
public:
	MatrixInversionPolicy() : _colinearity_detected(false) {}
	virtual ~MatrixInversionPolicy() {}
	virtual Matrix invert(const Matrix & m) = 0;

	bool colinearityDetected() const { return _colinearity_detected; }
	void setColinearityDetected() { _colinearity_detected = true; }

private:
	bool _colinearity_detected;
};

typedef boost::shared_ptr<MatrixInversionPolicy> MatrixInversionPolicyPtr;

	}
}

#endif //MR_STATISTICAL_ANALYSIS_MATRIX_INVERTER_H_INCLUDED
