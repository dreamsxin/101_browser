#include "MiniStdlib/MTAx_cstdio.h"
#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoder.h"

int main()
{
	char filename[] = "testfiles/T083v1_0/T83_process1/A1.jpg";

	FILE* jpegFile = MTAx_fopen(filename, "rb");

	if (jpegFile == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	Decode_image(jpegFile);

	fclose(jpegFile);
}
