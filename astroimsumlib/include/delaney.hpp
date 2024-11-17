#include "linalg.hpp"
#include "types.hpp"

namespace astro {
using triangle_t = static_array_t<index_t, 3>;

array_t<triangle_t> triangulate_points(array_t<point_t> points);
}
