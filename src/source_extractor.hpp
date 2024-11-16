#pragma once

#include "linalg.hpp"
#include "types.hpp"

class source_extractor {
public:
    array_t<point_t> extract(string_t file);
};
