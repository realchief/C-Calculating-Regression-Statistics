#include "Platform.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

#include "MRDateTbl.h"
#include "DebugNew.h"

using std::cerr;
using std::cout;
using std::endl;
using std::floor;
using std::pow;
using std::sin;
using std::sqrt;
using std::vector;

///////////////////////////////////////////////////////////////////////////////

/*****  Macros and Constants  *****/

//
// New moon times provided by NASA, measured in solar days since
// 1st January, 1995 (this is 727140 days from 1st March, 0004)
//
const double nasa_new_moon_times[] = {
    0.45625, 29.95138889, 59.49305556, 89.09097222, 118.7347222,
    148.3944444, 178.0354167, 207.6340278, 237.1881944, 266.7048611,
    296.1916667, 325.6548611, 355.0993056, 384.5361111, 413.9805556,
    443.4493056, 472.9520833, 502.4923611, 532.0680556, 561.6777778,
    591.3159722, 620.9631944, 650.5930556, 680.1777778, 709.7055556,
    739.1847222, 768.6298611, 798.0527778, 827.4611111, 856.8673611,
    886.2958333, 915.7791667, 945.3444444, 974.9951389, 1004.703472,
    1034.418056, 1064.09375, 1093.706944, 1123.251389, 1152.727778,
    1182.135417, 1211.488194, 1240.815278, 1270.161111, 1299.572917,
    1329.086111, 1358.710417, 1388.424306, 1418.186111, 1447.947222,
    1477.658333, 1507.278472, 1536.784722, 1566.183333, 1595.504167,
    1624.794444, 1654.100694, 1683.465278, 1712.91875, 1742.482639,
    1772.1625, 1801.939583, 1831.760417, 1861.545139, 1891.221528,
    1920.760417, 1950.176389, 1979.511111, 2008.806944, 2038.102083,
    2067.43125, 2096.829167, 2126.332639, 2155.967361, 2185.725,
    2215.547917, 2245.349306, 2275.058333, 2304.644444, 2334.116667,
    2363.499306, 2392.823611, 2422.122222, 2451.436111, 2480.808333,
    2510.278472, 2539.867361, 2569.563194, 2599.321528, 2629.086806,
    2658.807639, 2688.449306, 2717.991667, 2747.435417, 2776.802778,
    2806.132639, 2835.470833, 2864.858333, 2894.316667, 2923.850694,
    2953.452083, 2983.109028, 3012.80625, 3042.511111, 3072.18125,
    3101.777778, 3131.286806, 3160.727083, 3190.13125, 3219.535417,
    3248.958333, 3278.40625, 3307.880556, 3337.389583, 3366.947917,
    3396.558333, 3426.204861, 3455.853472, 3485.475694, 3515.058333,
    3544.603472, 3574.116667, 3603.602083, 3633.061806, 3662.502778,
    3691.9375, 3721.384028, 3750.857639, 3780.367361, 3809.915278,
    3839.503472, 3869.129167, 3898.781944, 3928.436111, 3958.058333,
    3987.625, 4017.133333, 4046.594444, 4076.022917, 4105.429167,
    4134.823611, 4164.227778, 4193.671528, 4223.188889, 4252.799306,
    4282.490278, 4312.21875, 4341.929167, 4371.584028, 4401.167361,
    4430.677083, 4460.113889, 4489.484722, 4518.811806, 4548.135417,
    4577.503472, 4606.961111, 4636.53125, 4666.209722, 4695.961111,
    4725.736806, 4755.484722, 4785.15625, 4814.71875, 4844.163889,
    4873.513194, 4902.808333, 4932.097222, 4961.426389, 4990.832639,
    5020.342361, 5049.96875, 5079.705556, 5109.516667, 5139.330556,
    5169.067361, 5198.672222, 5228.141667, 5257.508333, 5286.816667,
    5316.107639, 5345.418056, 5374.78125, 5404.23125, 5433.801389,
    5463.502083, 5493.300694, 5523.120139, 5552.877083, 5582.521528,
    5612.045833, 5641.469444, 5670.820139, 5700.13125, 5729.4375,
    5758.78125, 5788.202778, 5817.734028, 5847.378472, 5877.10625,
    5906.866667, 5936.606944, 5966.285417, 5995.877083, 6025.370833,
    6054.777778, 6084.127778, 6113.464583, 6142.83125, 6172.257639,
    6201.755556, 6231.320833, 6260.942361, 6290.610417, 6320.305556,
    6349.991667, 6379.627083, 6409.184028, 6438.663194, 6468.090972,
    6497.502083, 6526.922222, 6556.3625, 6585.822917, 6615.307639,
    6644.829167, 6674.402083, 6704.022222, 6733.665972, 6763.302778,
    6792.911111, 6822.483333, 6852.023611, 6881.534722, 6911.015278,
    6940.46875, 6969.903472, 6999.335417, 7028.783333, 7058.2625,
    7087.779861, 7117.340972, 7146.946528, 7176.592361, 7206.259722,
    7235.913889, 7265.522222, 7295.066667
};

const unsigned long NASA_NEW_MOON_COUNT = sizeof(nasa_new_moon_times) /
                                          sizeof(double);

//////////////////////////////////////////////////////////////////////////////

// Coefficients for the formulae currently used in bwdFunc() and fwdFunc()
namespace LunarCoefficients
{
    const double a = 29.53212586;
    const double b = 0.18122344;
    const double h = 0.086448246;
    const double d = 0.001905654;
    const double e = 23.14984645;
    const double f = 0.015302611;
    const double g = 24.29800328;
    const double i = -0.391605754;
};

///////////////////////////////////////////////////////////////////////////////

/*****  Global Variables  *****/

vector<unsigned __int64> new_moons;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

double
bwdFunc(double time)
{
    using namespace LunarCoefficients;
    return a + (b + h * sin(d * time + e + i)) * sin(f * time + g + i);
}

///////////////////////////////////////////////////////////////////////////////

double
fwdFunc(double time)
{
    using namespace LunarCoefficients;
    return a + (b + h * sin(d * time + e)) * sin(f * time + g);
}

///////////////////////////////////////////////////////////////////////////////

double
calcBwdStdDev()
{
    int i = 1;
    double total = 0;

    for (i = NASA_NEW_MOON_COUNT - 1 ; i > 0 ; i--) {
        double actual_period = nasa_new_moon_times[i] -
                               nasa_new_moon_times[i - 1];
        double calculated_period = bwdFunc(nasa_new_moon_times[i]);

        total += pow(actual_period - calculated_period, 2);
    }

    return sqrt(total) / (NASA_NEW_MOON_COUNT - 1);
}

///////////////////////////////////////////////////////////////////////////////

double
calcFwdStdDev()
{
    int i = 1;
    double total = 0;

    for (i = 1 ; i < NASA_NEW_MOON_COUNT ; i++) {
        double actual_period = nasa_new_moon_times[i] -
                               nasa_new_moon_times[i - 1];
        double calculated_period = fwdFunc(nasa_new_moon_times[i - 1]);

        total += pow(actual_period - calculated_period, 2);
    }

    return sqrt(total) / (NASA_NEW_MOON_COUNT - 1);
}

///////////////////////////////////////////////////////////////////////////////

unsigned __int64
convertNewMoonTime(double time)
{
    time += 727140;

    //
    // To get down to the date value, use the integer part of 'time' as the
    // index into the primary date table
    //
    unsigned long i = (unsigned long) floor(time);
    unsigned __int64 result = mr::DateTbl::prim_date_tbl[i];

    time -= i;          // Leave only the fractional part

    // Get minutes since 00:00 hrs
    double mins = time * 1440;

    // Round to nearest integer
    if ((mins - floor(mins)) >= 0.5) {
        mins = ceil(mins);
    }

    // Insert hours and minutes respectively
    result = (result * 100) + (unsigned __int64) (mins / 60);
    result = (result * 100) + (unsigned __int64) (std::fmod(mins, 60));

    result *= 100000;   // Insert zeros for seconds & milliseconds

    return result;
}

///////////////////////////////////////////////////////////////////////////////

void
calcNewMoons()
{
    double time = nasa_new_moon_times[0];

    // Calculate and add new moon times that come before the first one
    // provided by NASA (in reverse order)
    while (true) {
        time -= bwdFunc(time);

        if (time < -727140.0) {
            break;
        }

        new_moons.push_back(convertNewMoonTime(time));
    }

    std::reverse(new_moons.begin(), new_moons.end());   // Put in correct order

    // Add new moon times provided by NASA
    for (int i = 0 ; i < NASA_NEW_MOON_COUNT ; i++) {
        new_moons.push_back(convertNewMoonTime(nasa_new_moon_times[i]));
    }

    // Calculate and add new moon times that follow the last one provided by
    // NASA (in reverse order)
    time = nasa_new_moon_times[NASA_NEW_MOON_COUNT - 1];

    while (true) {
        time += fwdFunc(time);

        if (time > 733832) {
            break;
        }

        new_moons.push_back(convertNewMoonTime(time));
    }
}

///////////////////////////////////////////////////////////////////////////////

void
writeTable()
{
    unsigned int i = 0;

    cout << "#include \"MRDateTbl.h\"" << endl << endl
         << "////////////////////////////////////////////////////////////"
         << "///////////////////" << endl << endl
         << "/*****  Macros and Constants  *****/" << endl << endl
         << "//" << endl << "// Table statistics:" << endl
         << "// Standard Deviation of bwdFunc() on overlap with NASA data: "
         << calcBwdStdDev() << endl
         << "// Standard Deviation of fwdFunc() on overlap with NASA data: "
         << calcFwdStdDev() << endl
         << "// Number of new moons between 1 March 0004 and 1 March 4004: "
         << (unsigned int) new_moons.size() << endl << "//" << endl
         << "const unsigned long mr::DateTbl::lunar_date_tbl[] = {";

    for (i = 0 ; i < new_moons.size() ; i++) {
        if ((i % 8) == 0) {
            cout << endl << "    ";
        }

        cout << (new_moons[i] / 1000000000) << ',';
    }

    cout << endl << "};" << endl << endl
         << "const unsigned long mr::DateTbl::lunar_time_tbl[] = {";

    for (i = 0 ; i < new_moons.size() ; i++) {
        if ((i % 8) == 0) {
            cout << endl << "    ";
        }

        cout << (new_moons[i] % 1000000000) << ',';
    }

    cout << endl << "};" << endl << endl
         << "const unsigned long mr::DateTbl::LUNAR_TBL_SIZE = "
         << (unsigned int) new_moons.size() << ';' << endl << endl
         << "////////////////////////////////////////////////////////////"
         << "///////////////////" << endl;
}

///////////////////////////////////////////////////////////////////////////////

int
main(int argc,
     char *argv[])
{
    calcNewMoons();
    writeTable();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
