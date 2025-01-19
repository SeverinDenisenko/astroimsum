#include "summation.hpp"

#include "astrometric_reduction.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"
#include "threadpool.hpp"

namespace astro {
delaney_frame_summator::delaney_frame_summator(
    frame base_frame, uptr<source_extractor_interface> source_extractor)
    : base_frame_(base_frame)
    , source_extractor_(std::move(source_extractor))
{
    base_frame_points_ = source_extractor_->extract(base_frame_);
    std::cout << "imsum: "
              << "frame " << base_frame_.path().filename().string() << ": "
              << "found " << base_frame_points_.size() << " sources"
              << std::endl;
}

void delaney_frame_summator::sum(frame fr)
{
    uint32_t threads = 8;
    threadpool pool(threads);

    long w = fr.x_size();
    long h = fr.y_size();

    std::cout << "imsum: "
              << "generating transform from " << fr.path().filename().string()
              << " to " << base_frame_.path().filename().string() << std::endl;

    array_t<point_t> to = source_extractor_->extract(fr);
    std::cout << "imsum: "
              << "frame " << fr.path().filename().string() << ": "
              << "found " << to.size() << " sources" << std::endl;

    source_matcher matcher;

    pixel_transform_t transform = matcher.match(base_frame_points_, to);

    std::cout << "imsum: "
              << "x transform: " << matcher.get_transform_x()
              << "y transfrom: " << matcher.get_transform_y() << std::endl;

    std::cout << "imsum: "
              << "x error: " << matcher.get_error_x() << ", "
              << "y error: " << matcher.get_error_y() << std::endl;

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

    std::cout << "imsum: "
              << "frame " << fr.path().filename().string() << " done"
              << std::endl;
}

frame delaney_frame_summator::result()
{
    return base_frame_;
}

wcs_frame_summator::wcs_frame_summator(frame base_frame)
    : base_frame_(base_frame)
{
}

void wcs_frame_summator::sum(frame fr)
{
    // https://github.com/Punzo/wcslib/blob/master/C/test/twcstab.c
}

frame wcs_frame_summator::result()
{
    return base_frame_;
}

}
