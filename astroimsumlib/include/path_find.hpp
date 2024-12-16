#pragma once

#include "types.hpp"

namespace astro {

inline string_t path_find(array_t<string_t> variants)
{
    string_t res = "";

    for (string_t name : variants) {
        if (!system(("which " + name + " > /dev/null 2>&1").c_str())) {
            res = name;
            break;
        }
    }

    if (res == "") {
        throw std::runtime_error(
            "Can't find any of variants of " + variants[0]);
    }

    return res;
}

}
