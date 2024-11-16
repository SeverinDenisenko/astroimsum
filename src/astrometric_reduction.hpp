#pragma once

#include <functional>

#include "types.hpp"

using real_t = float;

struct coord_t {
    real_t x;
    real_t y;
};

using pixel_transform_t = std::function<coord_t(coord_t)>;

class source_extractor {
public:
    array_t<coord_t> extract();
};

class source_matcher {
public:
    pixel_transform_t match(array_t<coord_t> from, array_t<coord_t> to);
};

class ireduction {
public:
};
