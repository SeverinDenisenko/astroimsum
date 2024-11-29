#include "frame_io.hpp"

#include "fits.hpp"
#include "types.hpp"

namespace astro {
casual_framesaver::casual_framesaver(string_t out_name)
    : out_name_(out_name)
{
}

void casual_framesaver::save(const frame& fr)
{
    fits::image_hdu result_image
        = fits::image_hdu(index_array_t { fr.dimentions() });
    result_image.data = data_t(fr.data().begin(), fr.data().end());

    auto hdus = array_t<fits::hdu> { result_image };
    fits fits_out(out_name_, hdus, fr.cards());
}

batch_frameloader::batch_frameloader(array_t<string_t> files)
    : current_(0)
{
    for (auto& name : files) {
        fits fits_file(name);

        fits::image_hdu hdu = std::get<fits::image_hdu>(fits_file.hdus[0]);
        hdus_.emplace_back(hdu);

        array_t<string_t> cards       = fits_file.get_cards();
        unsigned_integer_t skip_cards = 10;

        frames_.emplace_back(
            hdus_.back().data,
            hdus_.back().naxes[0],
            hdus_.back().naxes[1],
            name,
            array_t<string_t>(cards.begin() + skip_cards, cards.end()));
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
