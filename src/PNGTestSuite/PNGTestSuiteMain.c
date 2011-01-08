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

#include "MiniStdlib/MTAx_cstdio.h"
#include <string.h>
#include <stdlib.h>
#include "PNG/PNG.h"

int main()
{
	char* filenames[] = {
		// Basic formats
		"basn0g01", // black & white
		"basn0g02", // 2 bit (4 level) grayscale
		"basn0g04", // 4 bit (16 level) grayscale
		"basn0g08", // 8 bit (256 level) grayscale
		"basn0g16", // 16 bit (64k level) grayscale
		"basn2c08", // 3x8 bits rgb color
		"basn2c16", // 3x16 bits rgb color
		"basn3p01", // 1 bit (2 color) paletted
		"basn3p02", // 2 bit (4 color) paletted
		"basn3p04", // 4 bit (16 color) paletted
		"basn3p08", // 8 bit (256 color) paletted
		"basn4a08", // 8 bit grayscale + 8 bit alpha-channel
		"basn4a16", // 16 bit grayscale + 16 bit alpha-channel
		"basn6a08", // 3x8 bits rgb color + 8 bit alpha-channel
		"basn6a16", // 16 bits rgb color + 16 bit alpha-channel

		
		// Interlacing
		"basi0g01", // black & white 
		"basi0g02", // 2 bit (4 level) grayscale 
		"basi0g04", // 4 bit (16 level) grayscale 
		"basi0g08", // 8 bit (256 level) grayscale 
		"basi0g16", // 16 bit (64k level) grayscale 
		"basi2c08", // 3x8 bits rgb color 
		"basi2c16", // 3x16 bits rgb color 
		"basi3p01", // 1 bit (2 color) paletted 
		"basi3p02", // 2 bit (4 color) paletted 
		"basi3p04", // 4 bit (16 color) paletted 
		"basi3p08", // 8 bit (256 color) paletted 
		"basi4a08", // 8 bit grayscale + 8 bit alpha-channel 
		"basi4a16", // 16 bit grayscale + 16 bit alpha-channel 
		"basi6a08", // 3x8 bits rgb color + 8 bit alpha-channel
		"basi6a16", // 3x16 bits rgb color + 16 bit alpha-channel



		// Corrupted files
		"x00n0g01", // empty 0x0 grayscale file 
		"xcrn0g04", // added cr bytes 
		"xlfn0g04"  // added lf bytes
	};

	size_t testCaseCount = sizeof(filenames)/sizeof(char*);

	char folder[]="testfiles/png/";
	size_t folderLen = sizeof(folder)-1;

	char folderWithFilename[sizeof(folder)+8+4];
	size_t i;

	strcpy(folderWithFilename, folder);

	for (i=0; i<testCaseCount; i++)
	{
		FILE* imgFile;
		ReadResult readResult;
		
		folderWithFilename[folderLen] = 0;
		strcat(folderWithFilename, filenames[i]);
		strcat(folderWithFilename, ".png");

		imgFile = MTAx_fopen(folderWithFilename, "rb");

		if (imgFile == NULL)
		{
			fprintf(stderr, "Could not open file %s\n", folderWithFilename);
			exit(1);
		}
		
		printf("%s\n", folderWithFilename);

		readResult = read_PNG(imgFile);

		if (i<testCaseCount-3)
		{
			if (readResult != ReadResultOK)
			{
				printf("Error in %s - expected OK\n", folderWithFilename);
			}
		}
		else
		{
			if (i == testCaseCount-3)
			{
				if (readResult != ReadResultPrematureEndOfStream)
				{
					printf("Error in %s - expected PrematureEndOfStream\n", folderWithFilename);
				}
			}
			else
			{
				if (readResult != ReadResultInvalidData)
				{
					printf("Error in %s - expected InvalidData\n", folderWithFilename);
				}
			}
		}

		fclose(imgFile);
	}
}
