#ifndef MR_STARISTICAL_ANALYSIS_MEAN_ACCUMULATOR_H_INCLUDED
#define MR_STARISTICAL_ANALYSIS_MEAN_ACCUMULATOR_H_INCLUDED

#ifdef _MSC_VER
#pragma once
#endif

namespace mr
{
	namespace statistical_analysis
	{
		struct MeanAccumulator
		{
			MeanAccumulator() :
				_sum(),
				_count()
			{
			}

			void operator()(double v)
			{
				if (!mr::math::isNan(v))
				{
					_sum += v;
					++_count;
				}
			}
			double getMean() const
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
		};
	}
}

#endif //MR_STARISTICAL_ANALYSIS_MEAN_ACCUMULATOR_H_INCLUDED
