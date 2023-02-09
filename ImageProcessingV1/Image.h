#pragma once

#include "Libs.h"

class Image
{
    std::vector<uint8_t> pixels_;
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

    void Save(const std::string& filename) const;
};