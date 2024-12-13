#include "frame_processing.hpp"
#include "types.hpp"

namespace astro {

void remove_tops(frame f, data_type_t threshold)
{
    for (integer_t i = 0; i < f.x_size(); ++i) {
        for (integer_t j = 0; j < f.y_size(); ++j) {
            if (f(i, j) > threshold) {
                f(i, j) = 0;
            }
        }
    }
}

}
