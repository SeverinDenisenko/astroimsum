#include "linalg.hpp"
#include "source_extractor.hpp"
#include "types.hpp"

using namespace astro;

int main()
{
    source_extractor extractor;

    array_t<point_t> points
        = extractor.extract("data/J75_01_20120806022018.fits");

    std::cout << points.size();

    for (point_t point : points) {
        std::cout << point[0] << " " << point[1] << std::endl;
    }

    return 0;
}
