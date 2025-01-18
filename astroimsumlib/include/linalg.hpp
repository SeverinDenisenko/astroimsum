#pragma once

#include <ios>
#include <ostream>
#include <stdexcept>

#include "types.hpp"

#define BOOST_UBLAS_NDEBUG

#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace astro {
namespace ublas = boost::numeric::ublas;

using real_t   = float;
using vector_t = ublas::vector<real_t>;
using matrix_t = ublas::matrix<real_t>;

inline std::ostream& operator<<(std::ostream& stream, const vector_t& vector)
{
    for (auto& x : vector) {
        stream << std::fixed << std::setw(4) << x << " ";
    }

    return stream;
}

using point_t = vector_t;

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline real_t dot(vector_t m, vector_t v)
{
    return ublas::inner_prod(m, v);
}

inline real_t norm(vector_t v)
{
    return ublas::norm_2(v);
}

inline matrix_t inverse(matrix_t m)
{
    matrix_t res = ublas::identity_matrix<real_t>(m.size1());

    ublas::permutation_matrix<size_t> pm(m.size1());
    ublas::lu_factorize(m, pm);
    ublas::lu_substitute(m, pm, res);

    return res;
}

inline matrix_t transpose(matrix_t m)
{
    return boost::numeric::ublas::trans(m);
}

inline matrix_t operator*(matrix_t a, matrix_t b)
{
    return ublas::prod(a, b);
}

inline vector_t operator*(matrix_t a, vector_t b)
{
    return ublas::prod(a, b);
}

inline vector_t operator*(vector_t a, real_t b)
{
    for (unsigned_integer_t i = 0; i < a.size(); ++i) {
        a[i] *= b;
    }
    return a;
}

inline vector_t vector_pow(vector_t a, real_t b)
{
    for (unsigned_integer_t i = 0; i < a.size(); ++i) {
        a[i] = pow(a[i], b);
    }
    return a;
}

inline vector_t vector_sqrt(vector_t a)
{
    for (unsigned_integer_t i = 0; i < a.size(); ++i) {
        a[i] = sqrt(a[i]);
    }
    return a;
}

inline real_t max(vector_t a)
{
    if (a.empty()) {
        throw std::runtime_error("max: vector is empty");
    }
    real_t m = a[0];
    for (unsigned_integer_t i = 0; i < a.size(); ++i) {
        m = std::max(m, a[i]);
    }
    return m;
}

inline unsigned_integer_t argmax(const vector_t& a)
{
    if (a.empty()) {
        throw std::runtime_error("argmax: vector is empty");
    }
    real_t m             = a[0];
    unsigned_integer_t r = 0;
    for (unsigned_integer_t i = 0; i < a.size(); ++i) {
        if (a[i] > m) {
            m = a[i];
            r = i;
        }
    }
    return r;
}

inline unsigned_integer_t argmin(const vector_t& a)
{
    if (a.empty()) {
        throw std::runtime_error("argmax: vector is empty");
    }
    real_t m             = a[0];
    unsigned_integer_t r = 0;
    for (unsigned_integer_t i = 0; i < a.size(); ++i) {
        if (a[i] < m) {
            m = a[i];
            r = i;
        }
    }
    return r;
}

inline matrix_t matrix(vector_t v)
{
    matrix_t m(v.size(), 1);
    std::copy(v.begin(), v.end(), m.begin1());
    return m;
}

}
