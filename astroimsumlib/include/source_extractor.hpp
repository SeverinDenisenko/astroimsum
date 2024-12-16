#pragma once

#include "frame.hpp"
#include "linalg.hpp"
#include "types.hpp"

namespace astro {

class source_extractor_interface {
public:
    virtual array_t<point_t> extract(frame f) = 0;
    virtual ~source_extractor_interface()     = default;
};

struct external_source_extractor_params {
    string_t path { "./sextractor/" };
};

class external_source_extractor : public source_extractor_interface {
public:
    external_source_extractor(external_source_extractor_params params);

    array_t<point_t> extract(frame f) override;

private:
    external_source_extractor_params params_;
};

struct embeded_source_extractor_params {
    integer_t detect_minarea { 10 };
    real_t detect_treshold { 3.0 };
    integer_t deblend_ntreshold { 28 };
    real_t deblend_mincount { 0.005 };
};

class embeded_source_extractor : public source_extractor_interface {
public:
    embeded_source_extractor(embeded_source_extractor_params params);

    array_t<point_t> extract(frame f) override;

private:
    embeded_source_extractor_params params_;
};
}
