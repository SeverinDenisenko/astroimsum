#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "fits.hpp"
#include "linalg.hpp"
#include "threadpool.hpp"
#include "types.hpp"

class frame {
private:
    data_span_t data_;
    index_t x_size_;
    index_t y_size_;

public:
    frame()
        : data_((unsigned long*)nullptr, 0)
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

    data_type_t& operator[](index_t x, index_t y)
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
};

class iframesaver {
public:
    virtual void save(const frame& fr) = 0;
};

class casual_framesaver : public iframesaver {
public:
    casual_framesaver(string_t out_name)
        : out_name_(out_name)
    {
    }

    void save(const frame& fr) override
    {
        fits::image_hdu result_image
            = fits::image_hdu(index_array_t { fr.dimentions() });
        result_image.bitpix = 32;
        result_image.data   = data_t(fr.data().begin(), fr.data().end());

        auto hdus = std::vector<fits::hdu> { result_image };
        fits fits_out(out_name_, hdus, array_t<string_t>());
    }

public:
    string_t out_name_;
};

class iframeloader {
public:
    virtual bool has_more()   = 0;
    virtual frame get_frame() = 0;
};

class batch_frameloader : public iframeloader {
public:
    batch_frameloader(array_t<string_t> files)
        : current_(0)
    {
        for (auto& name : files) {
            fits fits_file(name);

            fits::image_hdu hdu = std::get<fits::image_hdu>(fits_file.hdus[0]);
            hdus_.emplace_back(hdu);
        }

        for (auto& hdu : hdus_) {
            frames_.emplace_back(hdu.data, hdu.naxes[0], hdu.naxes[1]);
        }
    }

    bool has_more() override
    {
        return current_ < frames_.size();
    }

    virtual frame get_frame() override
    {
        auto current = current_;
        ++current_;
        return frames_[current];
    }

private:
    array_t<fits::image_hdu> hdus_;
    array_t<frame> frames_;
    unsigned_integer_t current_;
};

class isumattor {
public:
    virtual void sum(frame) = 0;
    virtual frame result()  = 0;
};

class basic_star_sumattor : public isumattor {
public:
    basic_star_sumattor(
        std::vector<std::pair<vector_t, vector_t>> models, frame base_frame)
        : base_frame_(base_frame)
        , current_(0)
        , models_(models)
    {
        std::fill(base_frame_.data().begin(), base_frame_.data().end(), 0);
    }

    virtual void sum(frame fr)
    {
        uint32_t threads = 8;
        threadpool pool(threads);

        long w = fr.x_size();
        long h = fr.y_size();

        auto model = models_[current_];
        ++current_;

        pool.run_parallel_works(
            [w, h, threads, this, &model, frame = fr](size_t thr) mutable {
                auto [Zx, Zy] = model;

                long size  = w / threads;
                long begin = size * thr;
                if (thr == threads - 1) {
                    size += w % size;
                }

                for (long i = begin; i < size + begin; ++i) {
                    for (long j = 0; j < h; ++j) {
                        long x = transform_pixel(i, j, Zx);
                        long y = transform_pixel(i, j, Zy);

                        if (x < 0 || y < 0) {
                            continue;
                        }

                        if (x >= w || y >= h) {
                            continue;
                        }

                        base_frame_[i, j] += frame[x, y];
                    }
                }
            });
    }

    virtual frame result()
    {
        return base_frame_;
    }

private:
    long transform_pixel(long x, long y, vector_t Z)
    {
        long Q        = Z.size();
        auto [x1, x2] = solve_square(1, 3, -2 * (Q - 1));
        long n        = std::round(std::max(x1, x2));
        long q        = 0;

        vector_t C(Q);
        std::fill(C.begin(), C.end(), 1);
        for (long i = 0; i <= n; ++i) {
            for (long j = 0; j <= n - i; ++j) {
                C[q] = pow(x, i) * pow(y, j);
                q += 1;
            }
        }

        return dot(C, Z);
    }

    std::pair<double, double> solve_square(double a, double b, double c)
    {
        double temp = -0.5 * (b + sgn(b) * sqrt(b * b - 4 * a * c));
        double x1   = temp / a;
        double x2   = c / temp;
        return { x1, x2 };
    }

    frame base_frame_;
    integer_t current_;
    std::vector<std::pair<vector_t, vector_t>> models_;
};

std::pair<vector_t, vector_t> get_model(long n)
{
    vector_t Zx(n);
    vector_t Zy(n);

    for (long i = 0; i < n; ++i) {
        std::cin >> Zx[i];
    }
    for (long i = 0; i < n; ++i) {
        std::cin >> Zy[i];
    }
    return { Zx, Zy };
}

static void sum_images(std::vector<std::string> images)
{
    std::vector<std::pair<vector_t, vector_t>> models;
    for (unsigned long i = 0; i < images.size(); ++i) {
        models.push_back(get_model(3));
    }

    sptr<iframeloader> frameloader = make_sptr<batch_frameloader>(images);

    frame base_frame = frameloader->get_frame();

    sptr<iframesaver> framesaver = make_sptr<casual_framesaver>("out.fits");
    sptr<isumattor> sumattor
        = make_sptr<basic_star_sumattor>(models, base_frame);

    while (frameloader->has_more()) {
        sumattor->sum(frameloader->get_frame());
    }

    framesaver->save(sumattor->result());
}

int main(int, char* argv[])
{
    std::string reference_image = argv[1];
    long images_count           = std::stoll(argv[2]);
    std::vector<std::string> images;
    images.push_back(reference_image);
    for (long i = 0; i < images_count; ++i) {
        images.push_back(argv[3 + i]);
    }

    sum_images(images);

    return 0;
}
