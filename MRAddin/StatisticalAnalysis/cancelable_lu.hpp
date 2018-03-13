//
// Modified version of boost lu.hpp
// to provide possibility to cancel the calculations
// and receive progress updates
//

#ifndef MR_STATISTICAL_ANALYSIS_BOOST_UBLAS_CANCELABLE_LU_
#define MR_STATISTICAL_ANALYSIS_BOOST_UBLAS_CANCELABLE_LU_

#include "Infrastructure/InfrastructureAPI.h"

// LU factorizations in the spirit of LAPACK and Golub & van Loan

namespace boost { namespace numeric { namespace ublas {

    // LU factorization without pivoting
    template<class M>
	typename M::size_type lu_cancelable_factorize (M &m, mr::infrastructure::CancelQuery cancel_query, mr::infrastructure::ProgressSink progress_sink) {
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;

#if BOOST_UBLAS_TYPE_CHECK
        matrix_type cm (m);
#endif
        int singular = 0;
        size_type size1 = m.size1 ();
        size_type size2 = m.size2 ();
        size_type size = (std::min) (size1, size2);
		
		progress_sink(0.0, "Matrix LU inversion");

        for (size_type i = 0; i < size; ++ i) {
			
			if (cancel_query() == true)
			{
				return singular;
			}
			
            
			matrix_column<M> mci (column (m, i));
            matrix_row<M> mri (row (m, i));
            if (m (i, i) != value_type/*zero*/()) {
                project (mci, range (i + 1, size1)) *= value_type (1) / m (i, i);
            } else if (singular == 0) {
                singular = i + 1;
            }
            project (m, range (i + 1, size1), range (i + 1, size2)).minus_assign (
                outer_prod (project (mci, range (i + 1, size1)),
                            project (mri, range (i + 1, size2))));

			progress_sink(static_cast<double>(i+1) / static_cast<double>(size), std::string());
        }
#if BOOST_UBLAS_TYPE_CHECK
        BOOST_UBLAS_CHECK (singular != 0 ||
                           detail::expression_type_check (prod (triangular_adaptor<matrix_type, unit_lower> (m),
                                                                triangular_adaptor<matrix_type, upper> (m)), 
                                                          cm), internal_logic ());
#endif
        return singular;
    }
}}}

#endif
