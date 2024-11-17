#include "delaney.hpp"

extern "C" {
#include "triangle.h"
}

namespace astro {
array_t<triangle_t> triangulate_points(array_t<point_t> points)
{
    struct triangulateio in, out;

    static_assert(std::is_same_v<REAL, real_t>);

    array_t<real_t> pointlist(points.size() * 2);
    for (unsigned_integer_t i = 0; i < points.size(); ++i) {
        pointlist[2 * i]     = points[i][0];
        pointlist[2 * i + 1] = points[i][1];
    }

    in.numberofpoints          = points.size();
    in.numberofpointattributes = 0;
    in.pointlist               = pointlist.data();
    in.pointattributelist      = nullptr;
    in.pointmarkerlist         = nullptr;
    in.numberofsegments        = 0;
    in.numberofholes           = 0;
    in.numberofregions         = 0;
    in.regionlist              = nullptr;

    out.numberofpoints          = 0;
    out.numberofpointattributes = 0;
    out.pointlist               = nullptr;
    out.pointattributelist      = nullptr;
    out.pointmarkerlist         = nullptr;
    out.numberofsegments        = 0;
    out.numberofholes           = 0;
    out.numberofregions         = 0;
    out.regionlist              = nullptr;
    out.numberoftriangles       = 0;
    out.trianglelist            = nullptr;

    // For debug
    // char flags[] = { 'V', 'z', 'D', 0 };
    char flags[] = { 'Q', 'z', 'D', 0 };

    triangulate(flags, &in, &out, nullptr);

    array_t<triangle_t> result;
    result.reserve(out.numberoftriangles);

    for (unsigned_integer_t i = 0;
         i < (unsigned_integer_t)out.numberoftriangles;
         ++i) {
        result.push_back(triangle_t { out.trianglelist[i * 3],
                                      out.trianglelist[i * 3 + 1],
                                      out.trianglelist[i * 3 + 2] });
    }

    free(out.trianglelist);

    return result;
}
}
