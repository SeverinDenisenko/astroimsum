#pragma once

#include "linalg.hpp"
#include "source_extractor.hpp"
#include "types.hpp"

#include <libconfig.hh>
#include <sstream>
#include <stdexcept>

namespace astro {
struct config {
    string_t source_extractor_type { "embeded" };
    embeded_source_extractor_params embeded_source_extractor {};
    external_source_extractor_params external_source_extractor {};
};

inline config read_config(string_t path)
{
    config c;

    try {
        libconfig::Config cfg;
        cfg.readFile(path);

        c.source_extractor_type
            = static_cast<string_t>(cfg.lookup("source_extractor"));
        c.embeded_source_extractor.detect_minarea = static_cast<integer_t>(
            cfg.lookup("embeded_source_extractor.detect_minarea"));
        c.embeded_source_extractor.detect_treshold = static_cast<real_t>(
            cfg.lookup("embeded_source_extractor.detect_treshold"));
        c.embeded_source_extractor.deblend_ntreshold = static_cast<integer_t>(
            cfg.lookup("embeded_source_extractor.deblend_ntreshold"));
        c.embeded_source_extractor.deblend_mincount = static_cast<real_t>(
            cfg.lookup("embeded_source_extractor.deblend_mincount"));
        c.external_source_extractor.path = static_cast<string_t>(
            cfg.lookup("external_source_extractor.path"));
    } catch (const libconfig::ParseException& ex) {
        std::stringstream err;
        err << "Cant pase config file:";
        err << " file=" << ex.getFile();
        err << " error = " << ex.getError();
        err << " line = " << ex.getLine();
        throw std::runtime_error(err.str());
    } catch (const libconfig::SettingNotFoundException& ex) {
        std::stringstream err;
        err << "Cant pase config file:";
        err << " path=" << ex.getPath();
        throw std::runtime_error(err.str());
    }

    return c;
}
}
