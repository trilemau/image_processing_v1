#pragma once

#include "Image.h"

class IImageProcessing
{
public:
    virtual ~IImageProcessing() = default;

    virtual std::string Info() const = 0;

    virtual Image Desaturate(const Image& image, int percent) = 0;
    virtual Image Greyscale(const Image& image) = 0;
    virtual Image Invert(const Image& image) = 0;
    virtual Image GaussianBlur(const Image& image, int times) = 0;
};