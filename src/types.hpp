#pragma once

#include <memory>
#include <span>
#include <vector>

template <typename T>
using uptr = std::unique_ptr<T>;

// Single-threaded shared pointer
template <typename T>
using sptr = std::__shared_ptr<T, __gnu_cxx::_S_single>;

template <typename T, typename... Args>
inline uptr<T> make_uptr(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
inline sptr<T> make_sptr(Args&&... args)
{
    return sptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
using array_t = std::vector<T>;

using string_t = std::string;

using integer_t          = ssize_t;
using unsigned_integer_t = size_t;

using data_type_t   = unsigned long;
using data_span_t   = std::span<data_type_t>;
using data_t        = array_t<data_type_t>;
using index_t       = long;
using index_array_t = array_t<index_t>;
