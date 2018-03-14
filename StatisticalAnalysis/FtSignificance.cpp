#include "Common.h"
#include "StatisticalAnalysis/FtSignificance.h"
#include "Infrastructure/MathUtils.h"
#include "DebugNew.h"

namespace mr
{
    namespace statistical_analysis
    {
        namespace
        {
            using infrastructure::math::isNaN;
            using infrastructure::math::NAN_NA;
        }
///////////////////////////////////////////////////////////////////////////////

//  The natural logarithm of the gamma function from NR page 214
//
//  Gamma(z) = Integrate[t^(z-1) e^(-t), t -> {0,Infinity}]
/// Reference: https://stats.stackexchange.com/questions/47840/linear-model-with-log-transformed-response-vs-generalized-linear-model-with-log
/// Reference: https://pdfs.semanticscholar.org/5c09/e09959e61c473034683a20621b62086d421b.pdf


double LnGamma(double x)
{
	double y, tmp, ser;
	static double adCoef[6]={76.18009172947146, -86.50532032941677, 24.01409824083091, -1.231739572450155, 0.1208650973866179e-2,-0.5395239384953e-5};
	y = x;
	tmp = x + 5.5;
	tmp -= (x + 0.5) * log(tmp);
	ser = 1.000000000190015;
	for (int j = 0; j < 6; j++)
	{
		y++;
		ser += adCoef[j]/y;
	}
	return -tmp + log(2.5066282746310005 * ser / x);
}

//  Calculates the continued fraction for IncompleteBeta.  From NR page 227
//
double BetaCompleteFraction(double a, double b, double x)
{
    int m,m2;
    double aa,c,d,del,h,qab,qam,qap;
    aa = 0;
    const int max_itr = 200;

    qab = a + b;
    qap = a + 1.0;
    qam = a - 1.0;
    c = 1.0;
    d = 1.0 - qab * x / qap;
    if (fabs(d) < DBL_MIN)
        d = DBL_MIN;
    d = 1.0/d;
    h = d;
    for (m = 1; m <= max_itr; m++)
    {
        m2 = 2 * m;
        aa = m * (b-m) * x / ((qam+m2)*(a+m2));
        d = 1.0 + aa * d;
        if (fabs(d) < DBL_MIN)
        d = DBL_MIN;
        c = 1.0 + aa/c;
        if (fabs(c) < DBL_MIN)
        c = DBL_MIN;
        d = 1.0/d;
        h *= d * c;
        aa = -(a+m) * (qab+m) * x / ( (a+m2) * (qap+m2) );
        d = 1.0 + aa * d;
        if (fabs(d) < DBL_MIN)
        d = DBL_MIN;
        c = 1.0 + aa/c;
        if (fabs(c) < DBL_MIN)
        c = DBL_MIN;
        d = 1.0/d;
        del=d*c;
        h *= del;
        if (fabs(del-1.0) < DBL_EPSILON)
        break;
    }
    return h;
}
//  The incomplete beta function from NR page 227
//
//  I(a,b,x) = 1/B(a,b)  Integrate[t^(a-1) (1-t)^(b-1),t -> {0,x}]
//
double IncompleteBeta(double a, double b, double x)
{
	if (x < 0.0 || x > 1.0)
    {
        float nan_na;
        *reinterpret_cast<unsigned long*>(&nan_na) = (0x7fe00020);
        return nan_na;
    }

	double bt;
	if (x == 0.0 || x == 1.0)
		bt = 0.0;
	else
		bt = exp( LnGamma(a+b) - LnGamma(a) - LnGamma(b) + a * log(x) + b * log(1.0 - x) );

	if (x < ((a + 1.0)/(a + b + 2.0)) )
		return bt * BetaCompleteFraction(a,b,x)/a;
	else
		return 1.0 - bt * BetaCompleteFraction(b,a,1.0 - x)/b;
}
//
//  The significance that the F ratio could be as large or larger due to sampling error
//  
//  f = ratio of df_num squared normally distributed numbers divided by df_den squared normally distributed numbers
//
double FSig(double f, double df_num, double df_den)
{
    double sig = IncompleteBeta( 0.5 * df_num, 0.5 * df_den, df_num / (df_num + f * df_den) );
	    if (sig > 1.0)
	    sig = 1.0 - sig;
    return sig;
}
///////////////////////////////////////////////////////////////////////////////

///
/// Return the square of x
///
double square( double x )
{
    return x * x;
}

//
//  The student-t significance that the t ratio of 
//  the mean of a normally distributed number divided by the standard error
//  is as as far from zero or farther by sampling error
//
double TSig(double t, double df)
{
    return FSig(square(t), df, 1.0);
}

//  Returns the probability that the means of the two samples are different based on Student's t-test
//  from NR page 617
double SigOfMeanDif(double mean_1, double std_dev_1, int count_1, double mean_2, double std_dev_2, int count_2)
{
	if (count_1 < 3 
    || (count_2 < 3) )
    {
		return 0.0;
    }

	if( isNaN(mean_1) 
    ||  isNaN(mean_2) 
    ||  isNaN(std_dev_1) 
    ||  isNaN(std_dev_2) )
    {
		return NAN_NA;
    }

    if( (std_dev_1 <= 0.0)
    ||  (std_dev_2 <= 0.0) )
    {
		return NAN_NA;
    }

	double var_1 = square(std_dev_1);
	double var_2 = square(std_dev_2);

	const double t = (mean_1 - mean_2) 
                   / sqrt((var_1/count_1) + (var_2/count_2));

	const double df = square((var_1/count_1) + (var_2/count_2)) 
                    / (square(var_1/count_1)/(count_1-1) + square(var_2/count_2)/(count_2-1));

	return IncompleteBeta( 0.5 * df, 0.5, df / (df + square(t)) );
}

double SigOfMeanDif(double mean_1, double std_dev_1, int count_1, double mean_2)
{
	if (count_1 < 3)
    {
		return 0.0;
    }

	if( isNaN(mean_1) 
    ||  isNaN(mean_2) 
    ||  isNaN(std_dev_1) )
    {
        return NAN_NA;
    }

    if( std_dev_1 <= 0.0)
    {
		return NAN_NA;
    }

	double t  = (mean_1 - mean_2) / ( std_dev_1 / sqrt(static_cast<double>(count_1)) );
	double df = count_1 - 1;
	return IncompleteBeta( 0.5 * df, 0.5, df / (df + square(t)) );
}
///////////////////////////////////////////////////////////////////////////////
double SigOfStDevDif(double std_dev_1, int count_1, double std_dev_2, int count_2)
{
	if (count_1 <= 3 || count_2 <= 3)
    {
		return 0.0;
    }

	if( isNaN(std_dev_1) || std_dev_1 <= 0.0 
    ||  isNaN(std_dev_2) || std_dev_2 <= 0.0 )
    {
		return NAN_NA;
    }

	double F, df_1, df_2, sig;

	double var_1 = std_dev_1 * std_dev_1;
	double var_2 = std_dev_2 * std_dev_2;

	if (var_1 > var_2)
	{
		F   = var_1 / var_2;
		df_1 = count_1 - 1;
		df_2 = count_2 - 1;
	}
	else
	{
		F   = var_2 / var_1;
		df_1 = count_2 - 1;
		df_2 = count_1 - 1;
	}

	sig = 2.0 * IncompleteBeta(0.5 * df_2, 0.5 * df_1, df_2 / (df_2 + df_1 * F));

	if (sig > 1.0)
		sig = 2.0 - sig;

	return sig;
}
///////////////////////////////////////////////////////////////////////////////

    }   // namespace statistical_analysis
}   // namespace mr
///////////////////////////////////////////////////////////////////////////////