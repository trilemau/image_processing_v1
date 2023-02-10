bool isValidPixel(int x, int y, int width, int height)
{
    return x >= 0 && x < width&& y >= 0 && y < height;
}

kernel void GaussianBlur(
    global unsigned char* input,
    global unsigned char* output,
    int width,
    int height,
    int bpp
)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    int red_sum = 0;
    int green_sum = 0;
    int blue_sum = 0;
    int count = 0;

    // Top left
    if (isValidPixel(x - 1, y - 1, width, height))
    {
        int i = (x - 1) * bpp + ((y - 1) * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Top middle
    if (isValidPixel(x, y - 1, width, height))
    {
        int i = x * bpp + ((y - 1) * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Top right
    if (isValidPixel(x + 1, y - 1, width, height))
    {
        int i = (x + 1) * bpp + ((y - 1) * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Middle left
    if (isValidPixel(x - 1, y, width, height))
    {
        int i = (x - 1) * bpp + (y * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Original pixel
    if (isValidPixel(x, y, width, height))
    {
        int i = x * bpp + (y * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Middle right
    if (isValidPixel(x + 1, y, width, height))
    {
        int i = (x + 1) * bpp + (y * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Bottom left
    if (isValidPixel(x - 1, y + 1, width, height))
    {
        int i = (x - 1) * bpp + ((y + 1) * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Bottom middle
    if (isValidPixel(x, y + 1, width, height))
    {
        int i = x * bpp + ((y + 1) * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    // Bottom right
    if (isValidPixel(x + 1, y + 1, width, height))
    {
        int i = (x + 1) * bpp + ((y + 1) * width * bpp);

        red_sum += input[i];
        green_sum += input[i + 1];
        blue_sum += input[i + 2];
        count++;
    }

    int i = x * bpp + (y * width * bpp);
    output[i] = red_sum / count;
    output[i + 1] = green_sum / count;
    output[i + 2] = blue_sum / count;
}
