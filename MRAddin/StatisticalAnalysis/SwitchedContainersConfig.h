#ifndef SWITCHED_CONTAINERS_CONFIG_H_GUARD_20160522
#define SWITCHED_CONTAINERS_CONFIG_H_GUARD_20160522

#pragma once

namespace mr
{
	namespace switched_models_common
	{
		struct SparseContainers
		{
			typedef boost::numeric::ublas::compressed_matrix<double> matrix_type;
			typedef boost::numeric::ublas::compressed_vector<double> vector_type;
			typedef boost::numeric::ublas::symmetric_matrix<double> symmetric_matrix_type;
		};

	}
}

#endif
