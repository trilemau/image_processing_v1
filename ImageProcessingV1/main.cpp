#include <chrono>

//#define IMAGE_PATH "C:/Users/stefq/Pictures/batman.jpg"
#define IMAGE_PATH "C:/Users/stefq/Pictures/314383957_1295843501270009_3371358039605884926_n.jpg"
#define ITERATIONS 20

#include "ImageProcessingCpp.h"
#include "ImageProcessingOpenCL.h"

// TODO list
// TODO PNG

int processImage(IImageProcessing& image_processing)
{
    auto start = std::chrono::system_clock::now();

    Image image(IMAGE_PATH);
    int width = image.GetWidth();
    int height = image.GetHeight();

    for (auto i = 0; i < ITERATIONS; i++)
    {
        std::cout << "Iteration=" << i << "\n";

        auto result = image_processing.GaussianBlur(image, 20);
    }

    auto end = std::chrono::system_clock::now();

    auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\n------" << image_processing.Info() << "------\n";
    std::cout << "TotalTime=" << difference.count() << "ms\n";
    std::cout << "AverageTime=" << difference.count() / ITERATIONS << "s\n";
    std::cout << "\n";

    return difference.count();
}

void comparison()
{
    ImageProcessingCpp image_processing_cpp;
    ImageProcessingOpenCL image_processing_opencl;


    auto time_cpp = processImage(image_processing_cpp);
    auto time_opencl = processImage(image_processing_opencl);

    std::cout << "multiple=" << time_cpp / time_opencl * 100 << "x\n";
}

void demo()
{
    Image image(IMAGE_PATH);
    int width = image.GetWidth();
    int height = image.GetHeight();

    //ImageProcessingCpp image_processing;
    ImageProcessingOpenCL image_processing;

    auto result = image_processing.Desaturate(image, 100);
    result.Save("desaturate.jpg");

    result = image_processing.Invert(image);
    result.Save("invert.jpg");

    result = image_processing.GaussianBlur(image, 50);
    result.Save("gaussian_blur.jpg");
}

int main()
{
    comparison();
    //demo();

    return 0;
}