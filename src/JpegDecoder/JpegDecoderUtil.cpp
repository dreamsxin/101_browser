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
#include "MiniStdlib/memory.h"  // for ENDIANNESS_CONVERT_SIMPLE
#include "MiniStdlib/cstdint.h" // for uint16_t
#include "MiniStdlib/safe_free.h"
#include "Util/ReadResult.h"

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

unsigned char readMarker(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface)
{
	unsigned char currentMarker;

	(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, &currentMarker, 1);

	if (currentMarker != 0xFF)
	{
		fprintf(stderr, "readMarker: expected token FF but received %2X\n", (unsigned int) currentMarker);
		longjmp(*in_out_pSetjmpStreamState->mpJmpBuffer, ReadResultInvalidData);
	}

	// Skip all 0xFF
	while (currentMarker == 0xFF)
	{
		(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, &currentMarker, 1);
	}

	printMarkerInformation(currentMarker);

	return currentMarker;
}

void defaultMarkerInterpreter(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	unsigned char currentMarker)
{
	if (!isStandaloneMarker(currentMarker))
	{
		uint16_t length;

		(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, &length, sizeof(length));

		ENDIANNESS_CONVERT_SIMPLE(length);

		printf("Length=%u", length);

		if (length<2)
		{
			fprintf(stderr, "defaultMarkerInterpreter: expected a length of at least 2\n");
			longjmp(*in_out_pSetjmpStreamState->mpJmpBuffer, ReadResultInvalidData);
		}

		uint8_t* data = (uint8_t*) malloc(length-2);

		if (!data)
			longjmp(*in_out_pSetjmpStreamState->mpJmpBuffer, ReadResultAllocationFailure);

		(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, data, length-2);

		safe_free(&data);
	}

	printf("\n");
}
