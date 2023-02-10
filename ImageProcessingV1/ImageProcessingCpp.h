#pragma once

#include "IImageProcessing.h"

class ImageProcessingCpp : public IImageProcessing
{
public:

    std::string Info() const override;

    Image Desaturate(const Image& image, int percent) override;
    Image Greyscale(const Image& image) override;
    Image Invert(const Image& image) override;
    Image GaussianBlur(const Image& image, int times) override;
};