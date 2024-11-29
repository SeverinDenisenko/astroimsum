#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gsl/span"

namespace astro {
template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T, typename... Args>
inline uptr<T> make_uptr(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
inline sptr<T> make_sptr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

using integer_t          = ssize_t;
using unsigned_integer_t = size_t;

template <typename T>
using array_t = std::vector<T>;

template <typename T, unsigned_integer_t I>
using static_array_t = std::array<T, I>;

using string_t = std::string;

using data_type_t   = float;
using data_t        = array_t<data_type_t>;
using data_span_t   = gsl::span<data_type_t>;
using index_t       = long;
using index_array_t = array_t<index_t>;

template <typename T>
using pair_t = std::pair<T, T>;
}
