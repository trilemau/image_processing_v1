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

Image ImageProcessing::GaussianBlur(const Image& image, int times)
{
    Image result = image;
    auto& pixels = result.GetPixels();

    auto width = image.GetWidth();
    auto height = image.GetHeight();
    auto bpp = image.GetBpp();

    for (int time = 0; time < times; time++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                int red_sum = 0;
                int green_sum = 0;
                int blue_sum = 0;
                int count = 0;

                // Top left
                if (image.IsValidPixel(x - 1, y - 1))
                {
                    int i = (x - 1) * bpp + ((y - 1) * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Top middle
                if (image.IsValidPixel(x, y - 1))
                {
                    int i = x * bpp + ((y - 1) * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Top right
                if (image.IsValidPixel(x + 1, y - 1))
                {
                    int i = (x + 1) * bpp + ((y - 1) * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Middle left
                if (image.IsValidPixel(x - 1, y))
                {
                    int i = (x - 1) * bpp + (y * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Original pixel
                if (image.IsValidPixel(x, y))
                {
                    int i = x * bpp + (y * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Middle right
                if (image.IsValidPixel(x + 1, y))
                {
                    int i = (x + 1) * bpp + (y * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Bottom left
                if (image.IsValidPixel(x - 1, y + 1))
                {
                    int i = (x - 1) * bpp + ((y + 1) * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Bottom middle
                if (image.IsValidPixel(x, y + 1))
                {
                    int i = x * bpp + ((y + 1) * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                // Bottom right
                if (image.IsValidPixel(x + 1, y + 1))
                {
                    int i = (x + 1) * bpp + ((y + 1) * width * bpp);

                    red_sum += pixels[i];
                    green_sum += pixels[i + 1];
                    blue_sum += pixels[i + 2];
                    count++;
                }

                int i = x * bpp + (y * width * bpp);
                pixels[i] = red_sum / count;
                pixels[i + 1] = green_sum / count;
                pixels[i + 2] = blue_sum / count;
            }
        }
    }

    return result;
}