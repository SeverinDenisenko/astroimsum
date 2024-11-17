#pragma once

#include <functional>

#include "linalg.hpp"
#include "types.hpp"

namespace astro {
using pixel_transform_t = std::function<point_t(point_t)>;

class source_matcher {
public:
    pixel_transform_t match(array_t<point_t> from, array_t<point_t> to);
};
}
