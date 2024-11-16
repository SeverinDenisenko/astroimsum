#pragma once

#include "frame.hpp"
#include "fits.hpp"

namespace astro {
    class iframesaver {
    public:
        virtual void save(const frame& fr) = 0;

        virtual ~iframesaver() = default;
    };

    class casual_framesaver : public iframesaver {
    public:
        casual_framesaver(string_t out_name);

        void save(const frame& fr) override;

    public:
        string_t out_name_;
    };

    class iframeloader {
    public:
        virtual bool has_more()   = 0;
        virtual frame get_frame() = 0;

        virtual ~iframeloader() = default;
    };

    class batch_frameloader : public iframeloader {
    public:
        batch_frameloader(array_t<string_t> files);

        bool has_more() override;

        frame get_frame() override;

    private:
        array_t<fits::image_hdu> hdus_;
        array_t<frame> frames_;
        unsigned_integer_t current_;
    };
}
