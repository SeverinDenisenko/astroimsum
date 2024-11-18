#include "astrometric_reduction.hpp"

#include "delaney.hpp"
#include "linalg.hpp"
#include "types.hpp"
#include <cstddef>

namespace astro {

static void
triangle(float x1, float y1, float x2, float y2, float x3, float y3, float* trp)
{
    float d12 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float d13 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
    trp[0]    = d13 / d12;
    trp[1]    = ((x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1)) / d13 / d12;
    trp[2]    = fabs((x2 - x1) * (y3 - y1) + (x3 - x1) * (y2 - y1)) / d13 / d12;
}

static void match(
    float* p1,
    float* p2,
    int n1,
    int n2,
    int* ID1,
    int* ID2,
    int nID1,
    int nID2,
    int* TRI1,
    int* TRI2)
{
    float* trp1 = new float[3];
    float* trp2 = new float[3];

    int Q[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
                    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };

    int Imin = 0, Jmin = 0, Kmin = 0;
    float d, dmin;
    for (int i = 0; i < nID1; i++) {
        int I    = ID1[3 * i];
        int J    = ID1[3 * i + 1];
        int K    = ID1[3 * i + 2];
        float x1 = p1[2 * I];
        float y1 = p1[2 * I + 1];
        float x2 = p1[2 * J];
        float y2 = p1[2 * J + 1];
        float x3 = p1[2 * K];
        float y3 = p1[2 * K + 1];
        triangle(x1, y1, x2, y2, x3, y3, trp1);
        for (int j = 0; j < nID2; j++) {
            for (int k = 0; k < 6; k++) {
                int I    = ID2[3 * j + Q[k][0]];
                int J    = ID2[3 * j + Q[k][1]];
                int K    = ID2[3 * j + Q[k][2]];
                float x1 = p2[2 * I];
                float y1 = p2[2 * I + 1];
                float x2 = p2[2 * J];
                float y2 = p2[2 * J + 1];
                float x3 = p2[2 * K];
                float y3 = p2[2 * K + 1];
                triangle(x1, y1, x2, y2, x3, y3, trp2);
                d = (trp1[0] - trp2[0]) * (trp1[0] - trp2[0])
                    + (trp1[1] - trp2[1]) * (trp1[1] - trp2[1])
                    + (trp1[2] - trp2[2]) * (trp1[2] - trp2[2]);
                if ((i == 0) && (j == 0) && (k == 0))
                    dmin = d;
                else {
                    if (d < dmin) {
                        Imin = i, Jmin = j, Kmin = k;
                        dmin = d;
                    }
                }
            }
        }
    }

    delete[] trp1;
    delete[] trp2;

    TRI1[0] = ID1[3 * Imin];
    TRI1[1] = ID1[3 * Imin + 1];
    TRI1[2] = ID1[3 * Imin + 2];
    TRI2[0] = ID2[3 * Jmin + Q[Kmin][0]];
    TRI2[1] = ID2[3 * Jmin + Q[Kmin][1]];
    TRI2[2] = ID2[3 * Jmin + Q[Kmin][2]];
}

static float transform(float x, float y, const float* Z, int n)
{
    int q   = 0;
    float u = 0;
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i + j <= n) {
                u += Z[q] * pow(x, i) * pow(y, j);
                q += 1;
            }
        }
    }
    return u;
}

static void overlap(
    float* IMG,
    float* img,
    int W,
    int H,
    int w,
    int h,
    float* Zx,
    float* Zy,
    int n)
{
    for (int x = 0; x < W; x++) {
        for (int y = 0; y < H; y++) {
            float u = transform(x, y, Zx, n);
            float v = transform(x, y, Zy, n);
            int U   = (int)u;
            int V   = (int)v;

            if ((U >= 0) && (U < w) && (V >= 0) && (V < h)) {
                IMG[y * W + x] = img[V * w + U];
            }
        }
    }
}

static void
overlapaf(float* IMG, float* img, int W, int H, int w, int h, float* Z)
{
    for (int x = 0; x < W; x++)
        for (int y = 0; y < H; y++) {
            float u = Z[1] + x * cos(Z[0]) + y * sin(Z[0]);
            float v = Z[2] - x * sin(Z[0]) + y * cos(Z[0]);
            int U   = (int)u;
            int V   = (int)v;

            if ((U >= 0) && (U < w) && (V >= 0) && (V < h)) {
                IMG[y * W + x] = img[V * w + U];
            }
        }
}

inline void overlap2(
    unsigned short* IMG,
    unsigned short* img,
    int W,
    int H,
    int w,
    int h,
    float* Zx,
    float* Zy,
    int n)
{
    for (int x = 0; x < W; x++) {
        for (int y = 0; y < H; y++) {
            float u = transform(x, y, Zx, n);
            float v = transform(x, y, Zy, n);
            int U   = (int)u;
            int V   = (int)v;

            if ((U >= 0) && (U < w) && (V >= 0) && (V < h)) {
                if (IMG[y * W + x] == 0) {
                    IMG[y * W + x] = img[V * w + U];
                } else {
                    IMG[y * W + x] = (IMG[y * W + x] + img[V * w + U]) / 2;
                }
            }
        }
    }
}

static real_t transform_pixel(real_t x, real_t y, vector_t Z)
{
    long Q = Z.size();
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

    return dot(C, Z);
}

pixel_transform_t
source_matcher::match(array_t<point_t> from, array_t<point_t> to)
{
    array_t<triangle_t> from_triangles = triangulate_points(from);
    array_t<triangle_t> to_triangles   = triangulate_points(to);

    static_array_t<int, 3> TRI1;
    static_array_t<int, 3> TRI2;

    array_t<real_t> p1(from.size() * 2);
    for (unsigned_integer_t i = 0; i < from.size(); ++i) {
        p1[2 * i]     = from[i][0];
        p1[2 * i + 1] = from[i][1];
    }

    array_t<real_t> p2(to.size() * 2);
    for (unsigned_integer_t i = 0; i < to.size(); ++i) {
        p2[2 * i]     = to[i][0];
        p2[2 * i + 1] = to[i][1];
    }

    array_t<int> ID1(from_triangles.size() * 3);
    for (unsigned_integer_t i = 0; i < from_triangles.size(); ++i) {
        ID1[3 * i]     = from_triangles[i][0];
        ID1[3 * i + 1] = from_triangles[i][1];
        ID1[3 * i + 2] = from_triangles[i][2];
    }

    array_t<int> ID2(to_triangles.size() * 3);
    for (unsigned_integer_t i = 0; i < to_triangles.size(); ++i) {
        ID2[3 * i]     = to_triangles[i][0];
        ID2[3 * i + 1] = to_triangles[i][1];
        ID2[3 * i + 2] = to_triangles[i][2];
    }

    ::astro::match(
        p1.data(),
        p2.data(),
        from.size(),
        to.size(),
        ID1.data(),
        ID2.data(),
        from_triangles.size(),
        to_triangles.size(),
        TRI1.data(),
        TRI2.data());

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
                for (unsigned_integer_t m = 0; m <= n; m++) {
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

    vector_t Zx;
    vector_t Zy;
    vector_t rx;
    vector_t ry;

    while (flag == 0) {
        Zx = inverse((transpose(C) * We) * C) * ((transpose(C) * We) * x2);
        Zy = inverse((transpose(C) * We) * C) * ((transpose(C) * We) * y2);
        rx = We * (x2 - C * Zx);
        ry = We * (y2 - C * Zy);
        vector_t r  = vector_sqrt(vector_pow(rx, 2) + vector_pow(ry, 2));
        real_t rmax = max(r);
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

    vector_t uwex
        = vector_sqrt((transpose(matrix(rx)) * We) * rx / (x1.size() - it - Q));
    vector_t uwey
        = vector_sqrt((transpose(matrix(ry)) * We) * ry / (y1.size() - it - Q));

    unsigned_integer_t nrs = x1.size() - it;

    // transform

    pixel_transform_t result = [Zx, Zy](point_t p) -> point_t {
        point_t res(2);
        res[0] = ::astro::transform_pixel(p[0], p[1], Zx);
        res[1] = ::astro::transform_pixel(p[0], p[1], Zy);
        return res;
    };

    return result;
}
}
