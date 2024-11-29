#include "fits.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <variant>
#include <vector>
#include <iostream>

#include "types.hpp"
#include "variant_utils.hpp"

namespace astro {
// Read fits
fits::fits(const string_t& file)
{
    fits_open_file(&fptr_, file.c_str(), READONLY, &status_);
    throw_fits_exception_if_needed();

    load_hdus();
    throw_fits_exception_if_needed();
};

fits::fits(const string_t& file, array_t<hdu>& hdus, array_t<string_t> cards)
{
    fits_create_file(&fptr_, file.c_str(), &status_);
    throw_fits_exception_if_needed();

    for (auto& card : cards) {
        fits_write_record(fptr_, card.c_str(), &status_);
        throw_fits_exception_if_needed();
    }

    create_hdus(hdus);
};

fits::~fits()
{
    fits_close_file(fptr_, &status_);
    // throw_fits_exception_if_needed();
    // DO NOT THROW EXCEPTION IN DESTRUCTOR!!!
    // Just ignore it.
}

array_t<string_t> fits::get_cards()
{
    int nkeys { 0 };
    fits_get_hdrspace(fptr_, &nkeys, NULL, &status_);
    throw_fits_exception_if_needed();

    array_t<string_t> cards;

    cards.reserve(nkeys);
    char card[FLEN_CARD];
    for (int i = 1; i <= nkeys; ++i) {
        fits_read_record(fptr_, i, card, &status_);
        throw_fits_exception_if_needed();

        cards.push_back(card);
    }

    return cards;
}

void fits::create_hdus(array_t<hdu>& hdus)
{
    for (auto& hdu : hdus) {
        std::visit(
            overloaded { [this](image_hdu& image) { create_image_hdu(image); },
                         [](ascii_hdu& ascii [[maybe_unused]]) {
                             throw std::runtime_error("Unsupported");
                         },
                         [](binary_hdu& binary [[maybe_unused]]) {
                             throw std::runtime_error("Unsupported");
                         } },
            hdu);
    }
}

void fits::create_image_hdu(image_hdu& image)
{
    long naxis = image.naxes.size();
    fits_create_img(fptr_, fits::image_hdu::dtype_bitpix, naxis, image.naxes.data(), &status_);
    throw_fits_exception_if_needed();

    index_array_t fpixel(naxis);
    std::fill(fpixel.begin(), fpixel.end(), 1);

    long nelements = std::accumulate(
        image.naxes.begin(), image.naxes.end(), 1, std::multiplies<long>());
    fits_write_pix(
        fptr_,
        image_hdu::dtype_enum,
        fpixel.data(),
        nelements,
        image.data.data(),
        &status_);
    throw_fits_exception_if_needed();
}

void fits::throw_fits_exception_if_needed()
{
    if (status_) {
        char errmsg[FLEN_ERRMSG];
        if (fits_read_errmsg(errmsg)) {
            throw std::runtime_error(errmsg);
        } else {
            throw std::runtime_error("can't get fits error");
        }
    }
}

void fits::load_current_hdu(int hdui [[maybe_unused]], int hdutype)
{
    hdu curr_hdu;
    if (hdutype == image_hdu::hdu_type_enum) {
        int bitpix { 0 };
        int naxis { 0 };
        fits_get_img_type(fptr_, &bitpix, &status_);
        fits_get_img_dim(fptr_, &naxis, &status_);
        throw_fits_exception_if_needed();
        if (naxis != 2) {
            throw std::runtime_error("Unsupported");
        }

        index_array_t naxes(naxis);
        fits_get_img_size(fptr_, naxis, naxes.data(), &status_);
        throw_fits_exception_if_needed();

        index_array_t fpixel(naxis);
        std::fill(fpixel.begin(), fpixel.end(), 1);
        long nelements = std::accumulate(
            naxes.begin(), naxes.end(), 1, std::multiplies<long>());
        image_hdu::dcontainer_t data(nelements);
        fits_read_pix(
            fptr_,
            image_hdu::dtype_enum,
            fpixel.data(),
            nelements,
            nullptr,
            data.data(),
            nullptr,
            &status_);
        throw_fits_exception_if_needed();

        curr_hdu = image_hdu(naxes, data);
    } else if (hdutype == ASCII_TBL) {
        curr_hdu = ascii_hdu {};
        throw std::runtime_error("Unsupported");
    } else if (hdutype == BINARY_TBL) {
        curr_hdu = binary_hdu {};
        throw std::runtime_error("Unsupported");
    }

    hdus.push_back(curr_hdu);
}

void fits::load_hdus()
{
    fits_get_num_hdus(fptr_, &hdunum_, &status_);

    hdus.reserve(hdunum_);
    for (int hdui = 1; hdui <= hdunum_; ++hdui) {
        int hdutype { 0 };
        fits_movabs_hdu(fptr_, hdui, &hdutype, &status_);
        throw_fits_exception_if_needed();

        load_current_hdu(hdui, hdutype);
    }
}
}
