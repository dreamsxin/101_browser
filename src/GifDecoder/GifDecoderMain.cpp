#include "BasicDataStructures/PlatformIndependentFunctions/fopen_s.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include "GraphicsDataStructures/Color.h"
using namespace std;

struct Header
{
	char Signature[3];
	char Version[3];
};

struct LogicalScreenDescriptor
{
	unsigned short LogicalScreenWidth;
	unsigned short LogicalScreenHeight;
	unsigned SizeOfGlobalColorTable : 3;
	unsigned SortFlag : 1;
	unsigned ColorResolution : 3;
	unsigned GlobalColorTableFlag : 1;
    unsigned char BackgroundColorIndex;
    unsigned char PixelAspectRatio;
};

struct GlobalColorTable
{
	vector<Color3<unsigned char> > colors;
};

size_t globalColorTableColorCount(LogicalScreenDescriptor logScrDescr)
{
	if (!logScrDescr.GlobalColorTableFlag)
		return 0;
	else
		return 1<<(logScrDescr.SizeOfGlobalColorTable+1);
}

int main()
{
	char filename[] = "testfiles/gif/GifSample.gif";
	FILE* imgFile = NULL;

	if (pi_fopen_s(&imgFile, filename, "rb") != 0)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	Header header;

	if (fread(&header, sizeof(Header), 1, imgFile) != 1)
	{
		fprintf(stderr, "Could not read header\n");
		exit(1);
	}

	if (strncmp(header.Signature, "GIF", 3)!=0)
	{
		fprintf(stderr, "Invalid signature. It has to be GIF\n");
		exit(1);
	}

	LogicalScreenDescriptor logScrDescr;

	if (fread(&logScrDescr, sizeof(LogicalScreenDescriptor), 1, imgFile) != 1)
	{
		fprintf(stderr, "Could not logical screen descriptor\n");
		exit(1);
	}

	size_t colorCount = globalColorTableColorCount(logScrDescr);

	if (colorCount != 0)
	{
		GlobalColorTable globalColorTable;
		globalColorTable.colors = vector<Color3<unsigned char> >();
		globalColorTable.colors.reserve(colorCount);
		for (size_t currentColorIndex = 0; currentColorIndex < colorCount; currentColorIndex++)
		{
			Color3<unsigned char> currentColor;
			if (fread(&currentColor, sizeof(Color3<unsigned char>), 1, imgFile) != 1)
			{
				fprintf(stderr, "Could not read color\n");
				exit(1);
			}
			globalColorTable.colors.push_back(currentColor);
		}
	}

	fclose(imgFile);
}
