#include "ImageProcessing.h"

Image ImageProcessing::Desaturate(const Image& image, int percent)
{
    float percent_float = static_cast<float>(percent) / 100;

    Image result = image;
    auto& pixels = result.GetPixels();

    for (auto i = 0; i < pixels.size(); i = i + 3)
    {
        auto& red = pixels[i];
        auto& green = pixels[i + 1];
        auto& blue = pixels[i + 2];

        float average = (red + green + blue) / 3;

        red = red - (red - average) * percent_float;
        green = green - (green - average) * percent_float;
        blue = blue - (blue - average) * percent_float;
    }

    return result;
}

Image ImageProcessing::Greyscale(const Image& image)
{
    return Desaturate(image, 100);
}

Image ImageProcessing::Invert(const Image& image)
{
    Image result = image;

    for (auto& pixel : result.GetPixels())
    {
        pixel = 255 - pixel;
    }

    return result;
}