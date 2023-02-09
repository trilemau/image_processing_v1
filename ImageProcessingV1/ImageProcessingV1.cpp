#pragma warning(disable: 4996)

#include <iostream>
#include <vector>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


//#define IMAGE_PATH "C:/Users/stefq/Pictures/batman.png"
#define IMAGE_PATH "C:/Users/stefq/Pictures/Untitled-3.jpg"
#define CHANNEL_NUM 3

struct Pixel
{
    int red_;
    int green_;
    int blue_;
};

class Image
{
    std::vector<uint8_t> pixels_;
    int width_;
    int height_;
    int bpp_;

public:
    Image(uint8_t* image_data, int width, int height, int bpp)
        : pixels_(image_data, image_data + width * height * bpp)
        , width_(width)
        , height_(height)
        , bpp_(bpp)
    {

    }

    //Image(const std::vector<uint8_t>& pixels, int width, int height, int bpp)
    //    : pixels_(pixels)
    //    , width_(width)
    //    , height_(height)
    //    , bpp_(bpp)
    //{

    //}

    const std::vector<uint8_t> GetPixels() const
    {
        return pixels_;
    }

    std::vector<uint8_t>& GetPixels()
    {
        return pixels_;
    }

    int getWidth() const
    {
        return width_;
    }

    int getHeight() const
    {
        return height_;
    }

    int getBpp() const
    {
        return bpp_;
    }
};

class ImageFilter
{
public:

    // TODO PNG

    static Image Desaturate(const Image& image, int percent)
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
    
    static Image Greyscale(const Image& image)
    {
        return Desaturate(image, 100);
    }
};

int main()
{
    int width, height, bpp;

    uint8_t* rgb_image = stbi_load(IMAGE_PATH, &width, &height, &bpp, CHANNEL_NUM);

    Image image(rgb_image, width, height, bpp);

    auto result = ImageFilter::Desaturate(image, 80);

    stbi_write_jpg("image.jpg", width, height, CHANNEL_NUM, result.GetPixels().data(), 100);
    //stbi_write_jpg("image.jpg", width, height, CHANNEL_NUM, rgb_image, 100);
    //stbi_write_png("image.jpg", width, height, CHANNEL_NUM, result.GetPixels().data(), width * CHANNEL_NUM);

    stbi_image_free(rgb_image);

    std::cout << IMAGE_PATH;
}