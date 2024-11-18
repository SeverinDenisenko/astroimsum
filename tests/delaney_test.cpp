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

    for(unsigned_integer_t i = 0; i < indexes.size(); ++i) {
        std::cout << indexes[i][0] << " " << indexes[i][1] << " " << indexes[i][2] << std::endl;
    }

    return 0;
}
