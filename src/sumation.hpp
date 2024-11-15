#pragma once

#include "frame.hpp"
#include "linalg.hpp"

class isumattor {
public:
    virtual void sum(frame) = 0;
    virtual frame result()  = 0;
};

class basic_star_sumattor : public isumattor {
public:
    basic_star_sumattor(
        std::vector<std::pair<vector_t, vector_t>> models, frame base_frame);

    virtual void sum(frame fr);

    virtual frame result();

private:
    long transform_pixel(long x, long y, vector_t Z);

    std::pair<double, double> solve_square(double a, double b, double c);

    frame base_frame_;
    integer_t current_;
    std::vector<std::pair<vector_t, vector_t>> models_;
};
