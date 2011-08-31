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

#include "JpegDecoder/JpegDecoderMarkers.h"
#include "JpegDecoder/JpegDecoderUtil.h"
#include "MiniStdlib/memory.h"  // for ENDIANNESS_CONVERT_SIMPLE
#include "MiniStdlib/cstdint.h" // for uint16_t
#include "MiniStdlib/safe_free.h"
#include "SetjmpUtil/ConditionalLongjmp.h"
#include "Util/ReadResult.h"

bool isStandaloneMarker(unsigned char in_marker)
{
	switch (in_marker)
	{
	case TEM_MARKER:   // 0x01
	case RST_0_MARKER: // 0xD0
	case RST_1_MARKER: // 0xD1
	case RST_2_MARKER: // 0xD2
	case RST_3_MARKER: // 0xD3
	case RST_4_MARKER: // 0xD4
	case RST_5_MARKER: // 0xD5
	case RST_6_MARKER: // 0xD6
	case RST_7_MARKER: // 0xD7
	case SOI_MARKER:   // 0xD8
	case EOI_MARKER:   // 0xD9
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
	case DQT_MARKER:    // 0xDB
		printf("DQT");
		break;
	case DRI_MARKER:    // 0xDD
		printf("DRI");
		break;
	case APP_0_MARKER:  // 0xE0
	case APP_1_MARKER:  // 0xE1
	case APP_2_MARKER:  // 0xE2
	case APP_3_MARKER:  // 0xE3
	case APP_4_MARKER:  // 0xE4
	case APP_5_MARKER:  // 0xE5
	case APP_6_MARKER:  // 0xE6
	case APP_7_MARKER:  // 0xE7
	case APP_8_MARKER:  // 0xE8
	case APP_9_MARKER:  // 0xE9
	case APP_10_MARKER: // 0xEA
	case APP_11_MARKER: // 0xEB
	case APP_12_MARKER: // 0xEC
	case APP_13_MARKER: // 0xED
	case APP_14_MARKER: // 0xEE
	case APP_15_MARKER: // 0xEF
		printf("APP_%u", in_marker-APP_0_MARKER);
		break;
	case COM_MARKER:    // 0xFE
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

	longjmpIf(currentMarker != 0xFF, in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
		ReadResultInvalidData, printHandler, "readMarker: expected token FF");

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

		SetjmpState allocationFailureSetjmpState;

		int result;
		jmp_buf freeMemoryJmpBuf;

		uint8_t* pData;

		(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, &length, sizeof(length));

		ENDIANNESS_CONVERT_SIMPLE(length);

		printf("Length=%u", length);

		longjmpIf(length < 2, in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
			ReadResultInvalidData, printHandler, "defaultMarkerInterpreter: expected a length of at least 2");

		setjmpStateInit(&allocationFailureSetjmpState, 
			in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
			ReadResultAllocationFailure, NULL);

		pData = (uint8_t*) setjmpStateLongjmpMalloc(
			&allocationFailureSetjmpState, length-2);

		if ((result = XCHG_AND_SETJMP(*in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
		freeMemoryJmpBuf)) != 0)
		{
			safe_free(&pData);
			xchgAndLongjmp(freeMemoryJmpBuf, 
				*in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, result);
		}

		(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, pData, length-2);

		safe_free(&pData);
		xchgJmpBuf(freeMemoryJmpBuf, *in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer);
	}

	printf("\n");
}
