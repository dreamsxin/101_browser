#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoder.h"

int main()
{
	char filename[] = //"testfiles/JPEG_example_JPG_RIP_100.jpg";
		"testfiles/T83/A1.jpg";

	char mode[] = "rb";

	FILE* jpegFile;

#ifdef _MSC_VER
	errno_t openResult = fopen_s(&jpegFile, filename, mode);

	if (openResult != 0)
#else
	jpegFile = fopen(filename, mode);

	if (!jpegFile)
#endif
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	Decode_image(jpegFile);

	fclose(jpegFile);
}
