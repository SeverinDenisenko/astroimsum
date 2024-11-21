#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "frame.hpp"
#include "frame_io.hpp"
#include "linalg.hpp"
#include "sumation.hpp"
#include "types.hpp"

namespace astro {
static void sum_images(array_t<string_t> images)
{
    sptr<iframeloader> frameloader = make_sptr<batch_frameloader>(images);

    frame base_frame = frameloader->get_frame();

    sptr<iframesaver> framesaver = make_sptr<casual_framesaver>("out.fits");
    sptr<isumattor> sumattor     = make_sptr<basic_star_sumattor>(base_frame);

    while (frameloader->has_more()) {
        sumattor->sum(frameloader->get_frame());
    }

    framesaver->save(sumattor->result());
}
}

int main(int argc, char* argv[])
{
    using namespace astro;

    try {
        if(argc < 3) {
            throw std::runtime_error("Wrong number of arguments");
        }

        string_t reference_image = argv[1];
        long images_count        = std::stoll(argv[2]);
        array_t<string_t> images;
        images.push_back(reference_image);
        for (long i = 0; i < images_count; ++i) {
            if(argc < i + 4) {
                throw std::runtime_error("Wrong number of arguments");
            }

            images.push_back(argv[3 + i]);
        }

        sum_images(images);
    } catch(const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
