#pragma once

#include "Libs.h"

struct Pixel
{
    Pixel(int red, int green, int blue)
        : red_(red)
        , green_(green)
        , blue_(blue)
    {

    }

    int red_;
    int green_;
    int blue_;
};

class Image
{
    std::vector<uint8_t> pixels_;
    std::vector<Pixel> pixels_rgb_;

    int width_;
    int height_;
    int bpp_;

public:
    constexpr static int CHANNEL_NUM = 3;

    Image(const std::string& path);

    const std::vector<uint8_t> GetPixels() const;
    std::vector<uint8_t>& GetPixels();
    int GetWidth() const;
    int GetHeight() const;
    int GetBpp() const;

    const Pixel& GetPixel(int x, int y) const;
    Pixel& GetPixel(int x, int y);
    bool IsValidPixel(int x, int y) const;

    void Save(const std::string& filename) const;

private:
    int GetArrayIndex(int x, int y) const;
};