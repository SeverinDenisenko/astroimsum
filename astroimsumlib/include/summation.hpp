#pragma once

#include "frame.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"

namespace astro {
class frame_summator_interface {
public:
    virtual void sum(frame)             = 0;
    virtual frame result()              = 0;
    virtual ~frame_summator_interface() = default;
};

class delaney_frame_summator : public frame_summator_interface {
public:
    delaney_frame_summator(
        frame base_frame, uptr<source_extractor_interface> source_extractor);

    virtual void sum(frame fr) override;

    virtual frame result() override;

private:
    frame base_frame_;
    uptr<source_extractor_interface> source_extractor_;
    array_t<point_t> base_frame_points_;
};

class wcs_frame_summator : public frame_summator_interface {
public:
    wcs_frame_summator(frame base_frame);

    virtual void sum(frame fr) override;

    virtual frame result() override;

private:
    frame base_frame_;
};

}
