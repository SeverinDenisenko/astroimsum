#pragma once

#include "types.hpp"

namespace astro {
    class frame {
    public:
        frame()
            : data_(nullptr, nullptr)
            , x_size_(0)
            , y_size_(0)
        {
        }

        frame(data_t& data, index_t x_size, index_t y_size)
            : data_(data.data(), data.size())
            , x_size_(x_size)
            , y_size_(y_size)
        {
        }

        data_type_t& operator()(index_t x, index_t y)
        {
            return data_[y * x_size_ + x];
        }

        index_t x_size() const
        {
            return x_size_;
        }

        index_t y_size() const
        {
            return y_size_;
        }

        index_array_t dimentions() const
        {
            return { x_size_, y_size_ };
        }

        const data_span_t data() const
        {
            return data_;
        }

    private:
        data_span_t data_;
        index_t x_size_;
        index_t y_size_;
    };
}
