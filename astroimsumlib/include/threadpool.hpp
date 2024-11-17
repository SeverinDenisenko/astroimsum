#pragma once

#include <functional>
#include <vector>

#include <boost/thread.hpp>

#include "types.hpp"

namespace astro {
class threadpool {
public:
    using work_t = std::function<void(long thr)>;

    threadpool(size_t threads)
        : threads_count_(threads)
    {
        threads_.reserve(threads_count_);
    }

    void run_parallel_works(work_t work)
    {
        for (size_t i = 0; i < threads_count_; ++i) {
            threads_.push_back(boost::thread([i, &work]() { work(i); }));
        }

        for (boost::thread& thread : threads_) {
            thread.join();
        }

        threads_.clear();
    }

private:
    array_t<boost::thread> threads_;
    size_t threads_count_;
};
}
