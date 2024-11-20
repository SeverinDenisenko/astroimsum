#pragma once

#include <functional>

#include "delaney.hpp"
#include "linalg.hpp"
#include "types.hpp"

namespace astro {
using pixel_transform_t = std::function<point_t(point_t)>;

class source_matcher {
public:
    pixel_transform_t
    match(const array_t<point_t>& from, const array_t<point_t>& to);
    const vector_t& get_error_x() const;
    const vector_t& get_error_y() const;
    vector_t get_transform_x() const;
    vector_t get_transform_y() const;

private:
    vector_t error_x_;
    vector_t error_y_;
    vector_t transform_x_;
    vector_t transform_y_;

    static real_t transform_pixel(real_t x, real_t y, vector_t transform);
    static pair_t<triangle_t> match(
        const array_t<point_t>& from,
        const array_t<point_t>& to,
        const array_t<triangle_t>& from_tris,
        const array_t<triangle_t>& to_tris);
};
}
