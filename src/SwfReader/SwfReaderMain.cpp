#include "BasicDataStructures/Stream/FileReadStream.h"
#include "BasicDataStructures/Stream/ZStream.h"
#include "SwfReader/SwfStructure.h"
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
	FileReadStream stream;

	if (!stream.open("testfiles/swf/gibbets.swf"))
		exit(1);

	SwfFile swfFile;

	if (stream.read(&swfFile.swfHeader1, sizeof(swfFile.swfHeader1), 1)!=1)
		exit(1);
	
	BasicReadStream* streamToContinue = NULL;

	bool compressed;

	if (strncmp(swfFile.swfHeader1.Signature, "CWS", 3)==0)
		compressed = true;
	else if (strncmp(swfFile.swfHeader1.Signature, "FWS", 3)==0)
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

	if (!readRECT(streamToContinue, &swfFile.swfHeader2.FrameSize))
		exit(1);

	if (streamToContinue->read(&swfFile.swfHeader2.FrameRate, sizeof(unsigned short), 2)!=2)
		exit(1);

	while (true)
	{
		Tag tag;

		unsigned short TagCodeAndLength;
		if (streamToContinue->read(&TagCodeAndLength, sizeof(unsigned short), 1)!=1)
		{
			break;
		}

		tag.recordHeader.TagCode = TagCodeAndLength>>6;
		unsigned short prematureLength = TagCodeAndLength & ((1<<6)-1);

		if (prematureLength == 0x3F)
		{
			if (streamToContinue->read(&tag.recordHeader.Length, sizeof(signed int), 1)!=1)
			{
				exit(1);
			}
		}
		else
		{
			tag.recordHeader.Length = prematureLength;
		}

		tag.tagContent = new unsigned char[tag.recordHeader.Length];

		if (streamToContinue->read(tag.tagContent, 1, tag.recordHeader.Length)!=tag.recordHeader.Length)
		{
			exit(1);
		}

		swfFile.tags.push_back(tag);
	}

	// Cleaning up

	if (compressed)
	{
		delete streamToContinue;
	}

	printf("Reading SWF succeeded\n");
	return 0;
}
