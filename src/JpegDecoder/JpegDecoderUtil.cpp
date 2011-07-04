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

#include <cstdlib>
#include "JpegDecoder/JpegDecoderMarkers.h"
#include "JpegDecoder/JpegDecoderUtil.h"

bool isStandaloneMarker(unsigned char in_marker)
{
	switch (in_marker)
	{
		// TEM
	case 0x01:
		// RST_0 to RST_7
	case 0xD0:
	case 0xD1:
	case 0xD2:
	case 0xD3:
	case 0xD4:
	case 0xD5:
	case 0xD6:
	case 0xD7:
	case SOI_MARKER: // 0xD8
	case EOI_MARKER: // 0xD9
		return true;
	default:
		return false;
	}
}

unsigned short flipBytes(unsigned short in_value)
{
	unsigned char lo = in_value & 0xFF;
	unsigned char hi = in_value >> 8;
	return (lo<<8)+hi;
}

void printSegmentName(unsigned char in_marker)
{
	switch (in_marker)
	{
	case SOF_0_MARKER:  // 0xC0
		printf("SOF_0");
		break;
	case SOF_1_MARKER:  // 0xC1
		printf("SOF_1");
		break;
	case SOF_2_MARKER:  // 0xC2
		printf("SOF_2");
		break;
	case SOF_3_MARKER:  // 0xC3
		printf("SOF_3");
		break;
	case DHT_MARKER:    // 0xC4
		printf("DHT");
		break;
	case SOF_9_MARKER:  // 0xC9
		printf("SOF_9");
		break;
	case SOF_10_MARKER: // 0xCA
		printf("SOF_10");
		break;
	case SOF_11_MARKER: // 0xCB
		printf("SOF_11");
		break;
	case SOF_55_MARKER: // 0xF7
		printf("SOF_55");
		break;
	case DAC_MARKER:    // 0xCC
		printf("DAC");
		break;
	case SOI_MARKER:    // 0xD8
		printf("SOI");
		break;
	case EOI_MARKER:    // 0xD9
		printf("EOI");
		break;
	case SOS_MARKER:    // 0xDA
		printf("SOS");
		break;
	case 0xDB:
		printf("DQT");
		break;
	case 0xDD:
		printf("DRI");
		break;
	case 0xE0:
		printf("APP_0");
		break;
	case 0xFE:
		printf("COM");
		break;
	default:
		printf("???");
		break;
	}
}


void printMarkerInformation(unsigned char currentMarker)
{
	printf("FF %2X (", currentMarker);
	printSegmentName(currentMarker);
	printf(")\n");
}

unsigned char readMarker(FILE* jpegFile)
{
	unsigned char currentMarker;

	if (fread(&currentMarker, 1, 1, jpegFile)!=1)
	{
		exit(1);
	}

	if (currentMarker != 0xFF)
	{
		fprintf(stderr, "Expected token FF but received %2X\n", (unsigned int) currentMarker);
		exit(1);
	}

	// Skip all 0xFF
	while (currentMarker == 0xFF)
	{
		if (fread(&currentMarker, 1, 1, jpegFile)!=1)
		{
			fprintf(stderr, "Expected token but received EOF\n");
			exit(1);
		}
	}

	printMarkerInformation(currentMarker);

	return currentMarker;
}

void defaultMarkerInterpreter(FILE* jpegFile, unsigned char currentMarker)
{
	if (!isStandaloneMarker(currentMarker))
	{
		unsigned short length;

		if (fread(&length, 2, 1, jpegFile)!=1)
		{
			fprintf(stderr, "Expected length parameter but received EOF\n");
			exit(1);
		}

		length = flipBytes(length);

		printf("Length=%u", length);

		if (length<2)
		{
			fprintf(stderr, "Expected a length of at least 2\n");
			exit(1);
		}

		unsigned char* data = (unsigned char*) malloc(length-2);

		if (!data)
		{
			fprintf(stderr, "Error allocating data\n");
			exit(1);
		}

		if (fread(data, length-2, 1, jpegFile)!=1)
		{
			fprintf(stderr, "Expected data of length %u but received EOF\n", length-2);
			exit(1);
		}

		free(data);
		data = NULL;
	}

	printf("\n");
}
