#include "fits.hpp"
#include "frame.hpp"
#include "frame_io.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"
#include "types.hpp"

using namespace astro;

int main()
{
    embeded_source_extractor extractor;
    batch_frameloader frameloader(
        array_t<string_t> { string_t("data/2015_HB10_H_alpha_003.fits") });
    frame f = frameloader.get_frame();

    array_t<point_t> points = extractor.extract(f);

    std::cout << points.size();

    for (point_t point : points) {
        std::cout << point[0] << " " << point[1] << std::endl;
    }

    return 0;
}
