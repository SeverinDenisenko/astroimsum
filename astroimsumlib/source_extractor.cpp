#include "source_extractor.hpp"
#include "frame_processing.hpp"
#include "linalg.hpp"
#include "types.hpp"

#include <boost/process.hpp>
#include <boost/process/pipe.hpp>

extern "C" {
#include "sep.h"
}

#include <cstddef>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace astro {
array_t<point_t> source_extractor::extract(string_t file)
{
    namespace bp = boost::process;

    array_t<string_t> names = { "sex", "sextractor", "source-extractor" };
    string_t sex            = "";
    for (string_t name : names) {
        if (!system(("which " + name + " > /dev/null 2>&1").c_str())) {
            sex = name;
            break;
        }
    }
    if (sex == "") {
        throw std::runtime_error("Can't find sextractor");
    }

    std::stringstream cmd;
    cmd << sex << " " << file << " -c "
        << "config.sex";

    bp::ipstream stdout;
    bp::ipstream stderr;
    bp::child child(cmd.str(), bp::std_out > stdout, bp::std_err > stderr);
    child.join();

    array_t<point_t> result;

    std::ifstream list("list.cat");
    string_t str;
    while (std::getline(list, str)) {
        if (str.front() == '#') {
            continue;
        }

        std::stringstream line(str);
        index_t index;
        point_t point(2);

        line >> index;
        line >> point[0];
        line >> point[1];

        result.push_back(point);
    }

    return result;
}

array_t<point_t> embeded_source_extractor::extract(frame frame)
{
    array_t<double> frame_copy(frame.data().begin(), frame.data().end());

    sep_image image = { frame_copy.data(),
                        NULL,
                        NULL,
                        NULL,
                        SEP_TDOUBLE,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        frame.x_size(),
                        frame.y_size(),
                        0.0,
                        SEP_NOISE_NONE,
                        1.0,
                        0.0 };

    sep_bkg* bkg = nullptr;
    int status   = sep_background(&image, 64, 64, 3, 3, 3.0, &bkg);
    if (status) {
        char errtext[512];
        sep_get_errdetail(errtext);

        throw std::runtime_error("Can't extract sources: " + string_t(errtext));
    }

    array_t<double> imback(frame.data().size());
    status = sep_bkg_array(bkg, imback.data(), SEP_TDOUBLE);
    if (status) {
        char errtext[512];
        sep_get_errdetail(errtext);

        throw std::runtime_error("Can't extract sources: " + string_t(errtext));
    }

    status = sep_bkg_subarray(bkg, frame_copy.data(), image.dtype);

    sep_catalog* catalog = nullptr;
    float conv[]         = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };

    status = sep_extract(
        &image,
        3.0 * bkg->globalrms,
        SEP_THRESH_ABS,
        10,
        conv,
        3,
        3,
        SEP_FILTER_CONV,
        28,
        0.005,
        1,
        1.0,
        &catalog);

    array_t<point_t> result(catalog->nobj);

    for (integer_t i = 0; i < catalog->nobj; i++) {
        result[i]    = point_t(2);
        result[i][0] = catalog->x[i];
        result[i][1] = catalog->y[i];
    }

    sep_catalog_free(catalog);
    sep_bkg_free(bkg);

    return result;
}
}
