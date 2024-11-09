#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "fits.hpp"
#include "linalg.hpp"
#include "threadpool.hpp"

class pipeline {
public:
    pipeline(
        std::string reference_image,
        std::vector<std::string> input_images_names_,
        std::string output_name_)
        : reference_image_name_(reference_image)
        , input_images_names_(input_images_names_)
        , output_name_(output_name_)
    {
    }

    void read_inputs()
    {
        for (auto& name : input_images_names_) {
            fits fits_file(name);

            fits::image_hdu image
                = std::get<fits::image_hdu>(fits_file.hdus[0]);

            images_.push_back(image);
        }

        fits reference(reference_image_name_);
        reference_image_ = std::get<fits::image_hdu>(reference.hdus[0]);

        cards_ = reference.get_cards();
        cards_.erase(cards_.begin(), cards_.begin() + 8);

        input_axes_   = reference_image_.naxes;
        input_bitpix_ = reference_image_.bitpix;
    }

    void create_output_image()
    {
        output_axes_   = input_axes_;
        output_bitpix_ = input_bitpix_;

        result_image_        = fits::image_hdu(output_axes_);
        result_image_.bitpix = output_bitpix_ * 2;
    }

    void summ_images(std::vector<std::pair<vector_t, vector_t>> models)
    {
        uint32_t threads = 8;
        threadpool pool(threads);

        long w = output_axes_[0];
        long h = output_axes_[1];

        for (size_t shot = 0; shot < images_.size(); ++shot) {
            pool.run_parallel_works(
                [w, h, shot, threads, this, &models](size_t thr) {
                    auto [Zx, Zy] = models[shot];
                    auto& image   = images_[shot];

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

                            result_image_(i, j) += image(x, y);
                        }
                    }
                });
        }
    }

    void write_result_image_()
    {
        auto hdus = std::vector<fits::hdu> { result_image_ };
        fits fits_out("out.fits", hdus, cards_);
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

    std::string reference_image_name_;
    std::vector<std::string> input_images_names_;
    std::vector<fits::image_hdu> images_;
    fits::image_hdu reference_image_;

    std::vector<std::string> cards_;
    fits::image_hdu result_image_;
    fits::image_hdu::axes_t input_axes_;
    fits::image_hdu::axes_t output_axes_;
    int input_bitpix_;
    int output_bitpix_;

    std::string output_name_;
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

int main(int, char* argv[])
{
    std::string reference_image = argv[1];
    long images_count           = std::stoll(argv[2]);
    std::vector<std::string> images;
    for (long i = 0; i < images_count; ++i) {
        images.push_back(argv[3 + i]);
    }

    std::vector<std::pair<vector_t, vector_t>> models;
    for (long i = 0; i < images_count; ++i) {
        models.push_back(get_model(3));
    }

    if (models.size() != images.size()) {
        throw std::runtime_error("Wrong");
    }

    pipeline pipeline(reference_image, images, "out.fits");
    pipeline.read_inputs();
    pipeline.create_output_image();
    pipeline.summ_images(models);
    pipeline.write_result_image_();

    return 0;
}
