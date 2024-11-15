#include "sumation.hpp"

#include "threadpool.hpp"

basic_star_sumattor::basic_star_sumattor(
    std::vector<std::pair<vector_t, vector_t>> models, frame base_frame)
    : base_frame_(base_frame)
    , current_(0)
    , models_(models)
{
    std::fill(base_frame_.data().begin(), base_frame_.data().end(), 0);
}

void basic_star_sumattor::sum(frame fr)
{
    uint32_t threads = 8;
    threadpool pool(threads);

    long w = fr.x_size();
    long h = fr.y_size();

    auto model = models_[current_];
    ++current_;

    pool.run_parallel_works(
        [w, h, threads, this, &model, frame = fr](size_t thr) mutable {
            auto [Zx, Zy] = model;

            long size  = w / threads;
            long begin = size * thr;
            if (thr == threads - 1) {
                size += w % size;
            }

            for (long i = begin; i < size + begin; ++i) {
                for (long j = 0; j < h; ++j) {
                    long x = transform_pixel(i, j, Zx);
                    long y = transform_pixel(i, j, Zy);

                    if (x < 0 || y < 0) {
                        continue;
                    }

                    if (x >= w || y >= h) {
                        continue;
                    }

                    base_frame_[i, j] += frame[x, y];
                }
            }
        });
}

frame basic_star_sumattor::result()
{
    return base_frame_;
}

long basic_star_sumattor::transform_pixel(long x, long y, vector_t Z)
{
    long Q        = Z.size();
    auto [x1, x2] = solve_square(1, 3, -2 * (Q - 1));
    long n        = std::round(std::max(x1, x2));
    long q        = 0;

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

std::pair<double, double>
basic_star_sumattor::solve_square(double a, double b, double c)
{
    double temp = -0.5 * (b + sgn(b) * sqrt(b * b - 4 * a * c));
    double x1   = temp / a;
    double x2   = c / temp;
    return { x1, x2 };
}
