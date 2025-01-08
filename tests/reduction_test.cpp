#include "astrometric_reduction.hpp"
#include "frame_io.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"

using namespace astro;

int main(int, char* argv[])
{
    batch_frameloader loader(array_t<string_t> { argv[1], argv[2] });
    frame f1 = loader.get_frame();
    frame f2 = loader.get_frame();

    external_source_extractor_params params { .path = "./sextractor/" };
    external_source_extractor extractor { params };
    array_t<point_t> from = extractor.extract(f1);
    array_t<point_t> to   = extractor.extract(f2);

    pixel_transform_t transform = source_matcher().match(from, to);

    point_t a(2);
    point_t b = transform(a);

    std::cout << b[0] << " " << b[1] << std::endl;

    return 0;
}
