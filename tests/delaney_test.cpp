#include "delaney.hpp"
#include "frame.hpp"
#include "frame_io.hpp"
#include "source_extractor.hpp"
#include "types.hpp"

using namespace astro;

int main()
{
    batch_frameloader loader(
        array_t<string_t> { "data/2015_HB10_H_alpha_002.fits" });
    frame f = loader.get_frame();

    external_source_extractor_params params { .path
                                              = "./sextractor/" };
    external_source_extractor extractor { params };

    array_t<point_t> points = extractor.extract(f);

    array_t<triangle_t> indexes = triangulate_points(points);

    std::cout << indexes.size() << std::endl;

    for (unsigned_integer_t i = 0; i < indexes.size(); ++i) {
        std::cout << indexes[i][0] << " " << indexes[i][1] << " "
                  << indexes[i][2] << std::endl;
    }

    return 0;
}
