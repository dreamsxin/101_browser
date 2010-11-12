#include "MiniStdlib/MTAx_cstdio.h"
#include "GIF/GIF.h"
#include <cstdlib> // for exit/(
using namespace std;

int main()
{
	char filename[] = 
		// "testfiles/gif/GifSampleSmall.gif";
		// "testfiles/gif/a_image.gif";
		// "testfiles/gif/GifSample.gif";
		// "testfiles/gif/Newtons_cradle_animation_book_2.gif";
		// "testfiles/gif/Rotating_earth_(large).gif";
		// "testfiles/gif/Rotating_earth_(small).gif";
		// "testfiles/gif/Sunflower_as_gif_small.gif";
		"testfiles/gif/tc217.gif"; // does not work at the moment

	FILE* imgFile = MTAx_fopen(filename, "rb");
	GIF_Data_Stream dataStream;

	if (imgFile == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	if (read_GIF_Data_Stream(imgFile, &dataStream) != ReadResultOK)
	{
		fprintf(stderr, "Could not read GIF file\n");
		exit(1);
	}

	fclose(imgFile);
}
