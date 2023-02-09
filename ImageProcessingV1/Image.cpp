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

void Image::Save(const std::string& filename) const
{
    stbi_write_jpg(filename.c_str(), width_, height_, Image::CHANNEL_NUM, pixels_.data(), 100);
}