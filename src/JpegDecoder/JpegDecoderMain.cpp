#include "MiniStdlib/MTAx_cstdio.h"
#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoder.h"

int main()
{
	char filename[] = "testfiles/T083/A1.jpg";

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
