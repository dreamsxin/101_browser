#include "MiniStdlib/MTAx_cstdio.h"
#include "GIF/GIF.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include "GraphicsDataStructures/Color.h"
using namespace std;



struct GlobalColorTable
{
	vector<Color3<unsigned char> > colors;
};

int main()
{
	char filename[] = // "testfiles/gif/GifSampleSmall.gif";
		"testfiles/gif/a_image.gif";

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
