#include "delaney.hpp"
#include "source_extractor.hpp"
#include "types.hpp"

using namespace astro;

int main()
{
    source_extractor extractor;
    array_t<point_t> points
        = extractor.extract("data/J75_01_20120806022018.fits");

    array_t<triangle_t> indexes = triangulate_points(points);

    std::cout << indexes.size() << std::endl;

    return 0;
}
