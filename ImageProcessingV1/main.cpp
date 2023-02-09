#define IMAGE_PATH "C:/Users/stefq/Pictures/batman.jpg"

#include "ImageProcessing.h"

int main()
{
    Image image(IMAGE_PATH);
    int width = image.GetWidth();
    int height = image.GetHeight();

    auto result = ImageProcessing::Desaturate(image, 100);
    result.Save("desaturate.jpg");

    result = ImageProcessing::Invert(image);
    result.Save("invert.jpg");

    result = ImageProcessing::GaussianBlur(image, 20);
    result.Save("gaussian_blur.jpg");
}