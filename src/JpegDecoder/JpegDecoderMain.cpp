#include "MiniStdlib/MTAx_cstdlib.h"
#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoder.h"

int main()
{
	char filename[] = //"testfiles/JPEG_example_JPG_RIP_100.jpg";
		"testfiles/T83/A1.jpg";

	char mode[] = "rb";

	FILE* jpegFile = MTAx_fopen(filename, mode);

	if (jpegFile == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	Decode_image(jpegFile);

	fclose(jpegFile);
}
