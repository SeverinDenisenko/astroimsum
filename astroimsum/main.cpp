#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "config.hpp"
#include "frame.hpp"
#include "frame_io.hpp"
#include "libconfig.h++"
#include "linalg.hpp"
#include "source_extractor.hpp"
#include "sumation.hpp"
#include "types.hpp"

#include <boost/program_options.hpp>

namespace astro {
static void sum_images(
    array_t<string_t> images, uptr<source_extractor_interface> source_extractor)
{
    sptr<iframeloader> frameloader = make_sptr<batch_frameloader>(images);

    frame base_frame = frameloader->get_frame();

    sptr<iframesaver> framesaver = make_sptr<casual_framesaver>("out.fits");
    sptr<isumattor> sumattor     = make_sptr<basic_star_sumattor>(
        base_frame, std::move(source_extractor));

    while (frameloader->has_more()) {
        sumattor->sum(frameloader->get_frame());
    }

    framesaver->save(sumattor->result());
}
}

int main(int argc, char* argv[])
{
    using namespace astro;

    namespace po = boost::program_options;

    try {
        po::options_description desc("imsum");
        desc.add_options()("help", "show this message")(
            "images",
            po::value<array_t<string_t>>()->multitoken(),
            "images to align and add, first image will be reference")(
            "config", po::value<string_t>(), "specify config file");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc;
            return 0;
        }

        if (!vm.count("images")) {
            std::cout << desc;
            return 0;
        }

        if (!vm.count("config")) {
            std::cout << desc;
            return 0;
        }

        string_t config_path     = vm["config"].as<string_t>();
        array_t<string_t> images = vm["images"].as<array_t<string_t>>();

        config c = read_config(config_path);

        uptr<source_extractor_interface> extractor;
        if (c.source_extractor_type == "embeded") {
            extractor = make_uptr<embeded_source_extractor>(
                c.embeded_source_extractor);
        } else {
            extractor = make_uptr<external_source_extractor>(
                c.external_source_extractor);
        }

        sum_images(images, std::move(extractor));
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
