kernel void Desaturate(
	global unsigned char* input,
	global unsigned char* output,
	int percent
)
{
	float percent_float = ((float)percent) / 100;
	int i = get_global_id(0) * 3;

	unsigned char red = input[i];
	unsigned char green = input[i+1];
	unsigned char blue = input[i+2];

	float average = (red + green + blue) / 3;

	output[i] = red - (red - average) * percent_float;
	output[i+1] = green - (green - average) * percent_float;
	output[i+2] = blue - (blue - average) * percent_float;
}
