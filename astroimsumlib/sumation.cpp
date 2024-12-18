#include "sumation.hpp"

#include "astrometric_reduction.hpp"
#include "frame_processing.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"
#include "threadpool.hpp"
#include <cstdio>

namespace astro {
basic_star_sumattor::basic_star_sumattor(frame base_frame)
    : base_frame_(base_frame)
    , current_(0)
{
    base_frame_points_ = embeded_source_extractor().extract(base_frame_);
    std::cout << base_frame_.name() << ": N = " << base_frame_points_.size()
              << std::endl;
}

void basic_star_sumattor::sum(frame fr)
{
    uint32_t threads = 8;
    threadpool pool(threads);

    long w = fr.x_size();
    long h = fr.y_size();

    std::cout << "Generating transform from " << fr.name() << " to "
              << base_frame_.name() << std::endl;

    array_t<point_t> to = embeded_source_extractor().extract(fr);
    std::cout << fr.name() << ": N = " << to.size() << std::endl;

    source_matcher matcher;

    pixel_transform_t transform = matcher.match(base_frame_points_, to);

    std::cout << matcher.get_transform_x() << std::endl;
    std::cout << matcher.get_transform_y() << std::endl;

    pool.run_parallel_works(
        [w, h, threads, this, transform, frame = fr](size_t thr) mutable {
            long size  = w / threads;
            long begin = size * thr;
            if (thr == threads - 1) {
                size += w % size;
            }

            for (long i = begin; i < size + begin; ++i) {
                for (long j = 0; j < h; ++j) {
                    point_t in(2);
                    in[0]       = i;
                    in[1]       = j;
                    point_t out = transform(in);
                    long x      = out[0];
                    long y      = out[1];

                    if (x < 0 || y < 0) {
                        continue;
                    }

                    if (x >= w || y >= h) {
                        continue;
                    }

                    base_frame_(i, j) += frame(x, y);
                }
            }
        });

    std::cout << "Frame " << fr.name() << " done" << std::endl;
}

frame basic_star_sumattor::result()
{
    return base_frame_;
}

}
