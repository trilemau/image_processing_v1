#pragma once

#include "Image.h"

class ImageProcessing
{
public:
    // TODO PNG

    static Image Desaturate(const Image& image, int percent);
    static Image Greyscale(const Image& image);
    static Image Invert(const Image& image);
};