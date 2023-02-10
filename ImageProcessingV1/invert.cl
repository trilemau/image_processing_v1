kernel void Invert(
	global unsigned char* input,
	global unsigned char* output
)
{
	output[get_global_id(0)] = 255 - input[get_global_id(0)];
}
