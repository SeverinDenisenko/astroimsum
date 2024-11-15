#pragma once

#include <memory>
#include <sys/types.h>
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
