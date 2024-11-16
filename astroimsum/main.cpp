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
    std::pair<vector_t, vector_t> get_model(long n)
    {
        vector_t Zx(n);
        vector_t Zy(n);

        for (long i = 0; i < n; ++i) {
            std::cin >> Zx[i];
        }
        for (long i = 0; i < n; ++i) {
            std::cin >> Zy[i];
        }
        return { Zx, Zy };
    }

    static void sum_images(array_t<string_t> images)
    {
        array_t<std::pair<vector_t, vector_t>> models;
        for (unsigned long i = 0; i < images.size(); ++i) {
            models.push_back(get_model(3));
        }

        sptr<iframeloader> frameloader = make_sptr<batch_frameloader>(images);

        frame base_frame = frameloader->get_frame();

        sptr<iframesaver> framesaver = make_sptr<casual_framesaver>("out.fits");
        sptr<isumattor> sumattor
            = make_sptr<basic_star_sumattor>(models, base_frame);

        while (frameloader->has_more()) {
            sumattor->sum(frameloader->get_frame());
        }

        framesaver->save(sumattor->result());
    }
}


int main(int, char* argv[])
{
    using namespace astro;

    string_t reference_image = argv[1];
    long images_count           = std::stoll(argv[2]);
    array_t<string_t> images;
    images.push_back(reference_image);
    for (long i = 0; i < images_count; ++i) {
        images.push_back(argv[3 + i]);
    }

    sum_images(images);

    return 0;
}
