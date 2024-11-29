#include "astrometric_reduction.hpp"

#include "delaney.hpp"
#include "linalg.hpp"
#include "types.hpp"
#include <cstddef>
#include <limits>

namespace astro {

static static_array_t<float, 3> triangle(point_t a, point_t b, point_t c)
{
    static_array_t<float, 3> res;
    float d12 = norm(b - a);
    float d13 = norm(c - a);
    res[0]    = d13 / d12;
    res[1]    = ((b[0] - a[0]) * (c[0] - a[0]) + (b[1] - a[1]) * (c[1] - a[1]))
        / d13 / d12;
    res[2] = fabs((b[0] - a[0]) * (c[1] - a[1]) + (c[0] - a[0]) * (b[1] - a[1]))
        / d13 / d12;
    return res;
}

pair_t<triangle_t> source_matcher::match(
    const array_t<point_t>& from,
    const array_t<point_t>& to,
    const array_t<triangle_t>& from_tris,
    const array_t<triangle_t>& to_tris)
{
    pair_t<triangle_t> result;

    int Q[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
                    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };

    int Imin = 0, Jmin = 0, Kmin = 0;
    float dmin = std::numeric_limits<float>::max();

    for (unsigned_integer_t i = 0; i < from_tris.size(); i++) {
        unsigned_integer_t I = from_tris[i][0];
        unsigned_integer_t J = from_tris[i][1];
        unsigned_integer_t K = from_tris[i][2];

        if (I >= to.size() || J >= to.size() || K >= to.size()) {
            continue;
        }

        static_array_t<float, 3> trp1 = triangle(from[I], from[J], from[K]);

        for (unsigned_integer_t j = 0; j < to_tris.size(); j++) {
            for (int k = 0; k < 6; k++) {
                unsigned_integer_t I = to_tris[j][Q[k][0]];
                unsigned_integer_t J = to_tris[j][Q[k][1]];
                unsigned_integer_t K = to_tris[j][Q[k][2]];

                if (I >= from.size() || J >= from.size() || K >= from.size()) {
                    continue;
                }

                static_array_t<float, 3> trp2 = triangle(to[I], to[J], to[K]);

                float d = (trp1[0] - trp2[0]) * (trp1[0] - trp2[0])
                    + (trp1[1] - trp2[1]) * (trp1[1] - trp2[1])
                    + (trp1[2] - trp2[2]) * (trp1[2] - trp2[2]);

                if (d < dmin) {
                    Imin = i, Jmin = j, Kmin = k;
                    dmin = d;
                }
            }
        }
    }

    result.first[0]  = from_tris[Imin][0];
    result.first[1]  = from_tris[Imin][1];
    result.first[2]  = from_tris[Imin][2];
    result.second[0] = to_tris[Jmin][Q[Kmin][0]];
    result.second[1] = to_tris[Jmin][Q[Kmin][1]];
    result.second[2] = to_tris[Jmin][Q[Kmin][2]];

    return result;
}

real_t source_matcher::transform_pixel(real_t x, real_t y, vector_t transform)
{
    long Q = transform.size();
    long n = 1;
    long q = 0;

    vector_t C(Q);
    std::fill(C.begin(), C.end(), 1);
    for (long i = 0; i <= n; ++i) {
        for (long j = 0; j <= n - i; ++j) {
            C[q] = pow(x, i) * pow(y, j);
            q += 1;
        }
    }

    return dot(C, transform);
}

pixel_transform_t
source_matcher::match(const array_t<point_t>& from, const array_t<point_t>& to)
{
    array_t<triangle_t> from_triangles = triangulate_points(from);
    array_t<triangle_t> to_triangles   = triangulate_points(to);

    pair_t<triangle_t> tris = match(from, to, from_triangles, to_triangles);
    triangle_t TRI1         = tris.first;
    triangle_t TRI2         = tris.second;

    vector_t X1(3);
    vector_t Y1(3);
    vector_t X2(3);
    vector_t Y2(3);

    X1[0] = from[TRI1[0]][0];
    X1[1] = from[TRI1[1]][0];
    X1[2] = from[TRI1[2]][0];

    Y1[0] = from[TRI1[0]][1];
    Y1[1] = from[TRI1[1]][1];
    Y1[2] = from[TRI1[2]][1];

    X2[0] = from[TRI2[0]][0];
    X2[1] = from[TRI2[1]][0];
    X2[2] = from[TRI2[2]][0];

    Y2[0] = from[TRI2[0]][1];
    Y2[1] = from[TRI2[1]][1];
    Y2[2] = from[TRI2[2]][1];

    matrix_t C(3, 3);
    C(0, 0) = 1;
    C(1, 0) = 1;
    C(2, 0) = 1;

    C(0, 1) = X2[0];
    C(1, 1) = X2[1];
    C(2, 1) = X2[2];

    C(0, 2) = Y2[0];
    C(1, 2) = Y2[1];
    C(2, 2) = Y2[2];

    vector_t ZX = inverse(C) * X1;
    vector_t ZY = inverse(C) * Y1;

    C = matrix_t(from.size(), 3);
    for (size_t i = 0; i < from.size(); ++i) {
        C(i, 0) = 1;
        C(i, 1) = from[i][0];
        C(i, 2) = from[i][1];
    }

    vector_t P1X = C * ZX;
    vector_t P1Y = C * ZY;

    // matchpos

    array_t<unsigned_integer_t> i1;
    array_t<unsigned_integer_t> I1;

    for (unsigned_integer_t k = 0; k < from.size(); ++k) {
        vector_t d(P1X.size());
        for (unsigned_integer_t j = 0; j < d.size(); ++j) {
            d[j] = sqrt(
                pow(P1X[j] - from[k][0], 2) + pow(P1Y[j] - from[k][1], 2));
        }

        real_t min                 = 1e100;
        unsigned_integer_t min_arg = 0;
        for (unsigned_integer_t j = 0; j < d.size(); ++j) {
            if (d[j] < min) {
                min     = d[j];
                min_arg = j;
            }
        }

        if (min < 5) {
            i1.push_back(k);
            I1.push_back(min_arg);
        }
    }

    vector_t x1(i1.size());
    vector_t y1(i1.size());
    vector_t x2(I1.size());
    vector_t y2(I1.size());

    for (unsigned_integer_t i = 0; i < i1.size(); ++i) {
        x1[i] = from[i1[i]][0];
        y1[i] = from[i1[i]][1];

        x2[i] = to[I1[i]][0];
        y2[i] = to[I1[i]][1];
    }

    unsigned_integer_t n    = 1;
    unsigned_integer_t Q    = (n + 1.0) * (n + 2.0) / 2.0;
    unsigned_integer_t Nmin = Q + 3;
    real_t rmax             = 0.5;

    // calibration

    C = matrix_t(x1.size(), Q);
    for (unsigned_integer_t i = 0; i < x1.size(); i++) {
        for (unsigned_integer_t j = 0; j < Q; j++) {
            C(i, j) = 1;
        }
    }

    unsigned_integer_t q = 0;
    for (unsigned_integer_t i = 0; i <= n; i++) {
        for (unsigned_integer_t j = 0; j <= n; j++) {
            if (i + j <= n) {
                for (unsigned_integer_t m = 0; m < x1.size(); m++) {
                    C(m, q) = pow(x1[m], i) * pow(y1[m], j);
                }
                q += 1;
            }
        }
    }

    matrix_t We(x1.size(), x1.size());
    for (unsigned_integer_t i = 0; i < We.size1(); i++) {
        for (unsigned_integer_t j = 0; j < We.size2(); j++) {
            if (i == j) {
                We(i, j) = 1;
            } else {
                We(i, j) = 0;
            }
        }
    }

    unsigned_integer_t flag = 0;
    unsigned_integer_t it   = 0;

    vector_t rx;
    vector_t ry;

    while (flag == 0) {
        transform_x_
            = inverse((transpose(C) * We) * C) * ((transpose(C) * We) * x2);
        transform_y_
            = inverse((transpose(C) * We) * C) * ((transpose(C) * We) * y2);

        rx          = We * (x2 - C * transform_x_);
        ry          = We * (y2 - C * transform_y_);
        vector_t r  = vector_sqrt(vector_pow(rx, 2) + vector_pow(ry, 2));
        real_t kmax = argmax(r);
        flag        = 1;
        if (size(x1) - it - Q <= Nmin) {
            break;
        }
        if (r[kmax] > rmax) {
            We(kmax, kmax) = 0;
            flag           = 0;
            it += 1;
        }
    }

    error_x_
        = vector_sqrt((transpose(matrix(rx)) * We) * rx / (x1.size() - it - Q));
    error_y_
        = vector_sqrt((transpose(matrix(ry)) * We) * ry / (y1.size() - it - Q));

    // transform

    pixel_transform_t result
        = [transform_x = this->transform_x_,
           transform_y = this->transform_y_](point_t p) -> point_t {
        point_t res(2);
        res[0] = transform_pixel(p[0], p[1], transform_x);
        res[1] = transform_pixel(p[0], p[1], transform_y);
        return res;
    };

    return result;
}

const vector_t& source_matcher::get_error_x() const
{
    return error_x_;
}

const vector_t& source_matcher::get_error_y() const
{
    return error_y_;
}

vector_t source_matcher::get_transform_x() const
{
    return transform_x_;
}

vector_t source_matcher::get_transform_y() const
{
    return transform_y_;
}

}
