#include "astrometric_reduction.hpp"
#include "linalg.hpp"
#include "source_extractor.hpp"

using namespace astro;

int main(int, char* argv[])
{
    source_extractor extractor;
    array_t<point_t> from = extractor.extract(argv[1]);
    array_t<point_t> to   = extractor.extract(argv[2]);

    pixel_transform_t transform = source_matcher().match(from, to);

    point_t a(2);
    point_t b = transform(a);

    std::cout << b[0] << " " << b[1] << std::endl;

    return 0;
}
