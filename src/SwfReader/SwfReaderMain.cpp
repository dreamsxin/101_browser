#include "BasicDataStructures/Stream/FileReadStream.h"
#include "BasicDataStructures/Stream/ZStream.h"
#include <cstdlib>
#include <cstring>

struct SwfHeader
{
	char Signature[3];
	unsigned char Version;
	unsigned int FileLength;
};

int main(int argc, char** argv)
{
	FileReadStream stream;

	if (!stream.open("testfiles/swf/watch-vfl127661.swf"))
		exit(1);

	SwfHeader header;

	if (stream.read(&header, sizeof(SwfHeader), 1)!=1)
		exit(1);
	
	BasicReadStream* streamToContinue = NULL;

	bool compressed;

	if (strncmp(header.Signature, "CWS", 3)==0)
		compressed = true;
	else if (strncmp(header.Signature, "FWS", 3)==0)
		compressed = false;
	else
	{
		exit(1);
	}

	if (compressed)
	{
		ZStream* zstream = new ZStream();
		zstream->open(&stream);
		streamToContinue = zstream;
	}
	else
	{
		streamToContinue = &stream;
	}

	unsigned char* restOfFile = new unsigned char[header.FileLength-sizeof(SwfHeader)];

	size_t inflatedBytes = streamToContinue->read(restOfFile, 1, header.FileLength-sizeof(SwfHeader));

	if (compressed)
	{
		delete streamToContinue;
	}

	delete[] restOfFile;

	if (inflatedBytes < header.FileLength-sizeof(SwfHeader))
		exit(1);

	printf("Reading SWF succeeded\n");
	return 0;
}
