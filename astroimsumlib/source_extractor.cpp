#include "source_extractor.hpp"
#include "frame.hpp"
#include "linalg.hpp"
#include "path_find.hpp"
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

external_source_extractor::external_source_extractor(
    external_source_extractor_params params)
    : params_(params)
{
}

array_t<point_t> external_source_extractor::extract(frame f)
{
    namespace bp = boost::process;
    namespace fs = boost::filesystem;

    array_t<string_t> names = { "sex", "sextractor", "source-extractor" };
    string_t sex            = path_find(names);
    string_t config_absolute
        = fs::canonical("config.sex", params_.path).string();
    string_t frame_absolute = fs::canonical(f.path()).string();

    array_t<string_t> args = { frame_absolute, "-c", config_absolute };

    bp::ipstream stdout;
    bp::ipstream stderr;
    bp::child child(
        bp::search_path(sex),
        args,
        bp::std_out > stdout,
        bp::std_err > stderr,
        bp::start_dir(params_.path));
    child.join();

    array_t<point_t> result;

    string_t result_absolute = fs::canonical("list.cat", params_.path).string();
    std::ifstream list(result_absolute);
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

embeded_source_extractor::embeded_source_extractor(
    embeded_source_extractor_params params)
    : params_(params)
{
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
        params_.detect_treshold * bkg->globalrms,
        SEP_THRESH_ABS,
        params_.detect_minarea,
        conv,
        3,
        3,
        SEP_FILTER_CONV,
        params_.deblend_ntreshold,
        params_.deblend_mincount,
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
