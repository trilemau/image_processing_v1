#include "Image.h"

#pragma warning(disable: 4996)

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


Image::Image(const std::string& path)
{
    uint8_t* image_data = stbi_load(path.c_str(), &width_, &height_, &bpp_, CHANNEL_NUM);

    pixels_ = std::vector<uint8_t>(image_data, image_data + width_ * height_ * bpp_);

    stbi_image_free(image_data);

    pixels_rgb_.reserve(width_ * height_);

    for (auto i = 0; i < pixels_.size(); i = i + bpp_)
    {
        pixels_rgb_.emplace_back(pixels_[i], pixels_[i + 1], pixels_[i + 2]);
    }
}

const std::vector<uint8_t> Image::GetPixels() const
{
    return pixels_;
}

std::vector<uint8_t>& Image::GetPixels()
{
    return pixels_;
}

int Image::GetWidth() const
{
    return width_;
}

int Image::GetHeight() const
{
    return height_;
}

int Image::GetBpp() const
{
    return bpp_;
}

const Pixel& Image::GetPixel(int x, int y) const
{
    const auto i = GetArrayIndex(x, y);

    if (i >= pixels_rgb_.size()) {
        throw std::out_of_range("Image::GetPixel(" + std::to_string(x) + ", " + std::to_string(y) + ") out of range " + std::to_string(width_) + "x" + std::to_string(height_));
    }

    return pixels_rgb_[i];
}

Pixel& Image::GetPixel(int x, int y)
{
    const auto i = GetArrayIndex(x, y);

    if (i >= pixels_rgb_.size()) {
        throw std::out_of_range("Image::GetPixel(" + std::to_string(x) + ", " + std::to_string(y) + ") out of range " + std::to_string(width_) + "x" + std::to_string(height_));
    }

    return pixels_rgb_[i];
}

bool Image::IsValidPixel(int x, int y) const
{
    return x >= 0 && x < width_&& y >= 0 && y < height_;
}

void Image::Save(const std::string& filename) const
{
    stbi_write_jpg(filename.c_str(), width_, height_, Image::CHANNEL_NUM, pixels_.data(), 100);
}

int Image::GetArrayIndex(int x, int y) const
{
    return x + (width_ * y);
}