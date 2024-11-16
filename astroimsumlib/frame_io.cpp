#include "frame_io.hpp"

#include "fits.hpp"

namespace astro {
    casual_framesaver::casual_framesaver(string_t out_name)
        : out_name_(out_name)
    {
    }

    void casual_framesaver::save(const frame& fr)
    {
        fits::image_hdu result_image
            = fits::image_hdu(index_array_t { fr.dimentions() });
        result_image.bitpix = 32;
        result_image.data   = data_t(fr.data().begin(), fr.data().end());

        auto hdus = array_t<fits::hdu> { result_image };
        fits fits_out(out_name_, hdus, array_t<string_t>());
    }

    batch_frameloader::batch_frameloader(array_t<string_t> files)
        : current_(0)
    {
        for (auto& name : files) {
            fits fits_file(name);

            fits::image_hdu hdu = std::get<fits::image_hdu>(fits_file.hdus[0]);
            hdus_.emplace_back(hdu);
        }

        for (auto& hdu : hdus_) {
            frames_.emplace_back(hdu.data, hdu.naxes[0], hdu.naxes[1]);
        }
    }

    bool batch_frameloader::has_more()
    {
        return current_ < frames_.size();
    }

    frame batch_frameloader::get_frame()
    {
        auto current = current_;
        ++current_;
        return frames_[current];
    }
}
