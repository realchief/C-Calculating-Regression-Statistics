#ifndef MR_STATISTICAL_ANALYSIS_COMMON_H
#define MR_STATISTICAL_ANALYSIS_COMMON_H

#include "..\mraddin\Platform.h"

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif
///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable : 4702 4267 4180)
#endif
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <cmath>
#include <cfloat>
#include <limits>

// STL includes
#include <vector>
#include <numeric>
#include <set>
#include <iomanip>
#include <list>
#include <deque>

// BOOST includes
#include <boost/bind.hpp>
#include <boost/cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/cstdint.hpp>
#include <boost/concept_check.hpp>
#include <boost/current_function.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/detail/algorithm.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>
#include "Common_cpp.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif
///////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_COMMON_H
///////////////////////////////////////////////////////////////////////////////
