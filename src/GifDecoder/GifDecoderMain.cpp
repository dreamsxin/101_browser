#include "MiniStdlib/MTAx_cstdio.h"
#include "GIF/GIF.h"
#include <cstdlib> // for exit()
using namespace std;

int main()
{
	char filename[] = 
		"testfiles/gif/wikipedia/GifSampleSmall.gif"; // works
		// "testfiles/gif/wikipedia/GifSample.gif"; // works
		// "testfiles/gif/wikipedia/Newtons_cradle_animation_book_2.gif"; // works
		// "testfiles/gif/wikipedia/Rotating_earth_(large).gif"; // !!!
		// "testfiles/gif/wikipedia/Rotating_earth_(small).gif"; // !!!
		// "testfiles/gif/wikipedia/Sunflower_as_gif_small.gif"; // !!!

		// "testfiles/gif/a_image.gif"; // works
		// "testfiles/gif/tc217.gif"; // works
		
		// "testfiles/gif/fileformat.info/GMARBLES.GIF"; // works
		// "testfiles/gif/fileformat.info/MARBLES.GIF"; // works
		// "testfiles/gif/fileformat.info/WFPC01.GIF"; // !!!
		// "testfiles/gif/fileformat.info/WFPC02.GIF"; // !!!
		// "testfiles/gif/fileformat.info/WFPC03.GIF"; // !!!
		// "testfiles/gif/fileformat.info/WFPC04.GIF"; // !!!
		// "testfiles/gif/fileformat.info/WFPC05.GIF"; // !!!
		// "testfiles/gif/fileformat.info/WFPC06.GIF"; // !!!

	FILE* imgFile = MTAx_fopen(filename, "rb");
	GIF_Data_Stream dataStream;
	ReadResult readResult;

	if (imgFile == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	readResult = read_GIF_Data_Stream(imgFile, &dataStream);

	if (readResult != ReadResultOK)
	{
		fprintf(stderr, "Could not read GIF file\n");
		exit(1);
	}

	fclose(imgFile);
}
