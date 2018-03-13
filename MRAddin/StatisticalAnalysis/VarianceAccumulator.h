#ifndef MR_STATISTICAL_ANALYSIS_VARIANCE_ACCUMULATOR_H_INCLUDED
#define MR_STATISTICAL_ANALYSIS_VARIANCE_ACCUMULATOR_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

namespace mr
{
	namespace statistical_analysis
	{
		struct VarianceAccumulator
		{
			explicit VarianceAccumulator(double mean) :
				_sum(),
				_count(),
				_mean(mean)
		{
		}

		void operator()(double v)
		{
			if (!mr::math::isNan(v))
			{
				_sum += pow(v - _mean, 2);
				++_count;
			}
		}
		double getVariance() const
		{
			if (_count != 0)
			{
				return _sum / _count;
			}
			else
			{
				return NAN_DIVBY0;
			}
		}
		private:
			double _sum;
			size_t _count;
			double _mean;
		};
	}
}

#endif
