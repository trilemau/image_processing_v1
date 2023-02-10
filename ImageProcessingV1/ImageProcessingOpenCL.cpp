#include "ImageProcessingOpenCL.h"

#include <fstream>

#define DESATURATE_OPENCL_FILE R"(C:/Users/stefq/Documents/GitHub/image_processing_v1/ImageProcessingV1/desaturate.cl)"
#define INVERT_OPENCL_FILE R"(C:/Users/stefq/Documents/GitHub/image_processing_v1/ImageProcessingV1/invert.cl)"
#define GAUSSIAN_BLUR_OPENCL_FILE R"(C:/Users/stefq/Documents/GitHub/image_processing_v1/ImageProcessingV1/gaussian_blur.cl)"

ImageProcessingOpenCL::ImageProcessingOpenCL()
{
    std::ifstream desaturate_file(DESATURATE_OPENCL_FILE);
    std::ifstream invert_file(INVERT_OPENCL_FILE);
    std::ifstream gaussian_blur_file(GAUSSIAN_BLUR_OPENCL_FILE);

    std::string desaturate_source{ std::istreambuf_iterator<char>(desaturate_file), std::istreambuf_iterator<char>() };
    std::string invert_source{ std::istreambuf_iterator<char>(invert_file), std::istreambuf_iterator<char>() };
    std::string gaussian_blur_source{ std::istreambuf_iterator<char>(gaussian_blur_file), std::istreambuf_iterator<char>() };

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    auto platform = platforms.front();
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices_);

    if (devices_.size() == 0) {
        throw std::runtime_error("OpenCL 0 devices");
    }

    device_ = devices_.front();
    context_ = cl::Context(device_);

    desaturate_source_ = cl::Program::Sources({ std::make_pair(desaturate_source.c_str(), desaturate_source.size() + 1) });
    invert_source_ = cl::Program::Sources({ std::make_pair(invert_source.c_str(), invert_source.size() + 1) });
    gaussian_blur_source_ = cl::Program::Sources({ std::make_pair(gaussian_blur_source.c_str(), gaussian_blur_source.size() + 1) });

    desaturate_program_ = cl::Program(context_, desaturate_source_);
    invert_program_ = cl::Program(context_, invert_source_);
    gaussian_blur_program_ = cl::Program(context_, gaussian_blur_source_);

    // Initialize programs
    auto error = desaturate_program_.build(devices_);

    if (error != 0) {
        throw std::runtime_error("OpenCL desaturate_program_ build error");
    }

    error = invert_program_.build();

    if (error != 0) {
        throw std::runtime_error("OpenCL invert_program_ build error");
    }

    error = gaussian_blur_program_.build();

    if (error != 0) {
        throw std::runtime_error("OpenCL gaussian_blur_program_ build error");
    }

    // Initialize queue
    queue_ = cl::CommandQueue(context_, device_);

    // Initialize kernels
    desaturate_kernel_ = cl::Kernel(desaturate_program_, "Desaturate", &error);

    if (error != 0) {
        throw std::runtime_error("OpenCL desaturate_kernel_ error");
    }

    invert_kernel_ = cl::Kernel(invert_program_, "Invert", &error);

    if (error != 0) {
        throw std::runtime_error("OpenCL invert kernel error");
    }

    gaussian_blur_kernel_ = cl::Kernel(gaussian_blur_program_, "GaussianBlur", &error);

    if (error != 0) {
        throw std::runtime_error("OpenCL gaussian blur kernel error");
    }
}

std::string ImageProcessingOpenCL::Info() const
{
    return "Image Processing using OpenCL";
}

Image ImageProcessingOpenCL::Desaturate(const Image& image, int percent)
{
    Image result = image;
    cl_int error;

    cl::Buffer input_buffer(context_, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(uint8_t) * result.GetPixels().size(), result.GetPixels().data(), &error);
    cl::Buffer output_buffer(context_, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR | CL_MEM_HOST_READ_ONLY, sizeof(uint8_t) * result.GetPixels().size(), result.GetPixels().data(), &error);

    error = desaturate_kernel_.setArg(0, input_buffer);
    error = desaturate_kernel_.setArg(1, output_buffer);
    error = desaturate_kernel_.setArg(2, percent);

    error = queue_.enqueueNDRangeKernel(desaturate_kernel_, cl::NullRange, cl::NDRange(image.GetWidth() * image.GetHeight()));
    queue_.enqueueMapBuffer(output_buffer, CL_TRUE, CL_MAP_READ, 0, sizeof(uint8_t) * result.GetPixels().size(), {}, {}, &error);

    return result;
}

Image ImageProcessingOpenCL::Greyscale(const Image& image)
{
    return Desaturate(image, 100);
}

Image ImageProcessingOpenCL::Invert(const Image& image)
{
    Image result = image;
    cl_int error;

    cl::Buffer input_buffer(context_, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(uint8_t) * result.GetPixels().size(), result.GetPixels().data(), &error);
    cl::Buffer output_buffer(context_, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR | CL_MEM_HOST_READ_ONLY, sizeof(uint8_t) * result.GetPixels().size(), result.GetPixels().data(), &error);

    error = invert_kernel_.setArg(0, input_buffer);
    error = invert_kernel_.setArg(1, output_buffer);

    error = queue_.enqueueNDRangeKernel(invert_kernel_, cl::NullRange, cl::NDRange(image.GetPixels().size()));
    queue_.enqueueMapBuffer(output_buffer, CL_TRUE, CL_MAP_READ, 0, sizeof(uint8_t) * result.GetPixels().size(), {}, {}, &error);

    return result;
}

Image ImageProcessingOpenCL::GaussianBlur(const Image& image, int times)
{
    Image result = image;
    cl_int error;

    for (auto n = 0; n < times; n++)
    {
        cl::Buffer input_buffer(context_, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(uint8_t) * result.GetPixels().size(), result.GetPixels().data(), &error);
        cl::Buffer output_buffer(context_, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR | CL_MEM_HOST_READ_ONLY, sizeof(uint8_t) * result.GetPixels().size(), result.GetPixels().data(), &error);

        error = gaussian_blur_kernel_.setArg(0, input_buffer);
        error = gaussian_blur_kernel_.setArg(1, output_buffer);
        error = gaussian_blur_kernel_.setArg(2, image.GetWidth());
        error = gaussian_blur_kernel_.setArg(3, image.GetHeight());
        error = gaussian_blur_kernel_.setArg(4, image.GetBpp());

        error = queue_.enqueueNDRangeKernel(gaussian_blur_kernel_, cl::NullRange, cl::NDRange(image.GetWidth(), image.GetHeight()));
        queue_.enqueueMapBuffer(output_buffer, CL_TRUE, CL_MAP_READ, 0, sizeof(uint8_t) * result.GetPixels().size(), {}, {}, &error);
    }

    return result;
}