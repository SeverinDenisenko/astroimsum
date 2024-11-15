#pragma once

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace ublas = boost::numeric::ublas;

using real_t   = double;
using vector_t = ublas::vector<real_t>;
using matrix_t = ublas::matrix<real_t>;

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline real_t dot(vector_t m, vector_t v)
{
    return ublas::inner_prod(m, v);
}
