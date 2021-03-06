/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "BasicDataStructures/Stream/BigEndianBitLittleEndianByteBitReadStream.h"
#include "BasicDataStructures/Stream/FileReadStream.h"
#include "BasicDataStructures/Stream/ZReadStream.h"
#include "SwfReader/SwfStructure.h"
#include <cstdlib>
#include <cstring>
#include "Template/Memory/SafeMemoryManagement.h"

using namespace std;

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
		ZReadStream* zreadstream = new ZReadStream();
		zreadstream->open(&stream);
		streamToContinue = zreadstream;
	}
	else
	{
		streamToContinue = &stream;
	}

	{
		BigEndianBitLittleEndianByteBitReadStream beblebbrs;
		beblebbrs.open(streamToContinue);

		if (beblebbrs.readBits(&swfFile.swfHeader2.FrameSize.NBits, 5, 1, 
			sizeof(swfFile.swfHeader2.FrameSize.NBits))!=1)
			exit(1);
		
		if (beblebbrs.readBits(&swfFile.swfHeader2.FrameSize.Xmin, 5, 4, 
			sizeof(swfFile.swfHeader2.FrameSize.Xmin))!=4)
			exit(1);
	}

	// read FrameRate and FrameCount
	if (streamToContinue->read(&swfFile.swfHeader2.FrameRate, sizeof(unsigned short), 2)!=2)
		exit(1);

	while (true)
	{
		SwfTag tag;

		unsigned short TagCodeAndLength;
		if (streamToContinue->read(&TagCodeAndLength, sizeof(unsigned short), 1)!=1)
		{
			break;
		}

		tag.recordHeader.TagType = TagCodeAndLength>>6;
		tag.recordHeader.Length = TagCodeAndLength & ((1<<6)-1);

		if (tag.recordHeader.Length == 0x3F)
		{
			if (streamToContinue->read(&tag.recordHeader.Length, sizeof(signed int), 1)!=1)
			{
				exit(1);
			}
		}

		tag.tagContent = new unsigned char[tag.recordHeader.Length];

		if (streamToContinue->read(tag.tagContent, 1, tag.recordHeader.Length)!=tag.recordHeader.Length)
		{
			exit(1);
		}

		swfFile.tags.push_back(tag);
	}

	// Cleaning up

	for (list<SwfTag>::iterator i=swfFile.tags.begin(); i!=swfFile.tags.end(); )
	{
		safe_delete_array(&i->tagContent);
		i=swfFile.tags.erase(i);
	}

	if (compressed)
	{
		safe_delete(&streamToContinue);
	}

	stream.close();

	printf("Reading SWF succeeded\n");
	return 0;
}
