#pragma once

#include "IImageProcessing.h"
#include <CL/cl.hpp>

class ImageProcessingOpenCL : public IImageProcessing
{
    std::vector<cl::Device> devices_;
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program::Sources desaturate_source_;
    cl::Program::Sources invert_source_;
    cl::Program::Sources gaussian_blur_source_;
    cl::Program desaturate_program_;
    cl::Program invert_program_;
    cl::Program gaussian_blur_program_;
    cl::Kernel desaturate_kernel_;
    cl::Kernel invert_kernel_;
    cl::Kernel gaussian_blur_kernel_;

public:

    ImageProcessingOpenCL();

    std::string Info() const override;

    Image Desaturate(const Image& image, int percent) override;
    Image Greyscale(const Image& image) override;
    Image Invert(const Image& image) override;
    Image GaussianBlur(const Image& image, int times) override;
};