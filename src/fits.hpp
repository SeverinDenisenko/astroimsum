#pragma once

#include <numeric>
#include <span>
#include <stdexcept>
#include <variant>
#include <vector>

#include "fitsio.h"

#include "types.hpp"

using data_type_t   = unsigned long;
using data_span_t   = std::span<data_type_t>;
using data_t        = array_t<data_type_t>;
using index_t       = long;
using index_array_t = array_t<index_t>;

class fits {
public:
    struct image_hdu {
        using dtype_t                      = unsigned long;
        using dcontainer_t                 = std::vector<dtype_t>;
        using axes_t                       = std::vector<long>;
        static constexpr int hdu_type_enum = IMAGE_HDU;
        static constexpr int dtype_enum    = TULONG;

        image_hdu() = default;
        image_hdu(axes_t axes)
            : naxes(axes)
            , data(std::accumulate(
                  naxes.begin(), naxes.end(), 1, std::multiplies<long>()))
        {
        }
        image_hdu(int bitpix, axes_t naxes, dcontainer_t data)
            : bitpix(bitpix)
            , naxes(naxes)
            , data(data)
        {
        }

        int bitpix { 64 };
        axes_t naxes {};
        dcontainer_t data {};

        dtype_t& operator()(long x, long y)
        {
            if (x >= naxes[0] || y >= naxes[1]) {
                throw std::runtime_error("Out of bounds");
            }
            return data[x + naxes[0] * y];
        }
    };

    class ascii_hdu {
    public:
        ascii_hdu()
        {
        }
    };

    class binary_hdu {
    public:
        binary_hdu()
        {
        }
    };

    using hdu = std::variant<image_hdu, ascii_hdu, binary_hdu>;

    // Read fits
    fits(const std::string& file);

    // Write fits
    fits(
        const std::string& file,
        std::vector<hdu>& hdus,
        std::vector<std::string> cards);

    ~fits();

    std::vector<std::string> get_cards();

    std::vector<hdu> hdus;

private:
    void create_hdus(std::vector<hdu>& hdus);

    void create_image_hdu(image_hdu& image);

    void throw_fits_exception_if_needed();

    void load_current_hdu(int hdui [[maybe_unused]], int hdutype);

    void load_hdus();

    fitsfile* fptr_ { nullptr };
    int hdunum_ { 0 };
    int status_ { 0 };
};
