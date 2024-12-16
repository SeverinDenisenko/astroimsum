#pragma once

#include "frame.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"

namespace astro {
class isumattor {
public:
    virtual void sum(frame) = 0;
    virtual frame result()  = 0;
    virtual ~isumattor()    = default;
};

class basic_star_sumattor : public isumattor {
public:
    basic_star_sumattor(
        frame base_frame, uptr<source_extractor_interface> source_extractor);

    virtual void sum(frame fr);

    virtual frame result();

private:
    long transform_pixel(long x, long y, vector_t Z);

    std::pair<double, double> solve_square(double a, double b, double c);

    frame base_frame_;
    uptr<source_extractor_interface> source_extractor_;
    integer_t current_;
    array_t<point_t> base_frame_points_;
};
}
