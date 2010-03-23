#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoder.h"

int main()
{
	char filename[] = //"testfiles/JPEG_example_JPG_RIP_100.jpg";
		"testfiles/T83/A1.jpg";
	FILE* jpegFile = fopen(filename, "rb");

	if (!jpegFile)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	Decode_image(jpegFile);

	fclose(jpegFile);
}
