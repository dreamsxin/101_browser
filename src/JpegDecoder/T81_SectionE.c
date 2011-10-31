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

#include "JpegDecoder/T81_SectionE.h"
#include "MiniStdlib/MTAx_cstdlib.h"
#include "MiniStdlib/cassert.h"
#include "MiniStdlib/cstring.h" // for memset
#include "JpegDecoder/JpegDecoderMarkers.h"
#include "JpegDecoder/JpegDecoderUtil.h"
#include "JpegDecoder/JpegSegmentsRead.h"
#include "SetjmpUtil/ConditionalLongjmp.h"

void Decoder_setup(JpegContext *in_pContext)
{
	memset(in_pContext, 0, sizeof(JpegContext));
}

void interpreteMarkersFromTableE1(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	uint8_t in_currentMarker, JpegContext *in_pJpegContext)
{
	switch (in_currentMarker)
	{
	case DRI_MARKER:    // 0xDD
		readRestartInterval(in_out_pSetjmpStreamState, 
			in_setjmpStreamReadInterface, 
			&in_pJpegContext->restartIntervalState);
		break;
	case DHT_MARKER:    // 0xC4
	case DAC_MARKER:    // 0xCC
	case DQT_MARKER:    // 0xDB
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
	case COM_MARKER:    // 0xFE
		defaultMarkerInterpreter(in_out_pSetjmpStreamState, 
			in_setjmpStreamReadInterface, 
			in_currentMarker);
		break;
	default:
		longjmpWithHandler(in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
			ReadResultInvalidData, printHandler, 
			"interpreteMarkersFromTableE1: invalid marker in Decode_image");
	}
}

// E.2.1 Control procedure for decoding compressed image data
ReadResult Decode_image(void *in_pStreamState, 
	ByteStreamInterface in_byteStreamReadInterface)
{
	uint8_t currentMarker;
	JpegContext jpegContext;

	SetjmpStreamState setjmpReadStreamState;
	ByteStreamInterface setjmpReadStreamInterface;
	jmp_buf jmpBuf;
	int result;

	setjmpStreamInit(&setjmpReadStreamState, &jmpBuf, ReadResultPrematureEndOfStream, 
		in_pStreamState, in_byteStreamReadInterface);
	setjmpReadStreamInterface = getSetjmpStreamByteStreamInterface(&setjmpReadStreamState);

	if ((result = setjmp(jmpBuf)) != 0)
		return (ReadResult) result;

	currentMarker = readMarker(&setjmpReadStreamState, setjmpReadStreamInterface);

	if (currentMarker != SOI_MARKER)
	{
		longjmpWithHandler(setjmpReadStreamState.setjmpState.mpJmpBuffer, ReadResultInvalidData, 
			printHandler, "Decode_image: expected SOI marker");
	}

	// SOI is a standalone marker
	assert(isStandaloneMarker(currentMarker));

	Decoder_setup(&jpegContext);

	currentMarker = readMarker(&setjmpReadStreamState, setjmpReadStreamInterface);

	while (
		currentMarker != SOF_0_MARKER  && 
		currentMarker != SOF_1_MARKER  && 
		currentMarker != SOF_2_MARKER  && 
		currentMarker != SOF_3_MARKER  && 
		currentMarker != SOF_5_MARKER  && 
		currentMarker != SOF_6_MARKER  && 
		currentMarker != SOF_7_MARKER  && 
		currentMarker != SOF_9_MARKER  && 
		currentMarker != SOF_10_MARKER && 
		currentMarker != SOF_11_MARKER &&
		currentMarker != SOF_13_MARKER &&
		currentMarker != SOF_14_MARKER &&
		currentMarker != SOF_15_MARKER &&
		currentMarker != SOF_55_MARKER)
	{
		interpreteMarkersFromTableE1(&setjmpReadStreamState, 
			setjmpReadStreamInterface, currentMarker, &jpegContext);

		currentMarker = readMarker(&setjmpReadStreamState, setjmpReadStreamInterface);
	}

	Decode_frame(&setjmpReadStreamState, setjmpReadStreamInterface, 
		currentMarker, &jpegContext);

	return ReadResultOK;
}

// E.2.2 Control procedure for decoding a frame
void Decode_frame(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	uint8_t currentMarker, JpegContext *in_pJpegContext)
{
	// TODO: Replace by "Interpret frame header"
	defaultMarkerInterpreter(in_out_pSetjmpStreamState, in_setjmpStreamReadInterface, currentMarker);
	currentMarker = readMarker(in_out_pSetjmpStreamState, in_setjmpStreamReadInterface);

	while (currentMarker != SOS_MARKER)
	{
		interpreteMarkersFromTableE1(in_out_pSetjmpStreamState, 
			in_setjmpStreamReadInterface, currentMarker, 
			in_pJpegContext);

		currentMarker = readMarker(in_out_pSetjmpStreamState, in_setjmpStreamReadInterface);
	}

	Decode_scan(in_out_pSetjmpStreamState, in_setjmpStreamReadInterface, in_pJpegContext);

	// TODO: further markers can occur before EOI
}

// E.2.3 Control procedure for decoding a scan
void Decode_scan(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	JpegContext *in_pJpegContext)
{
	size_t m = 0;

	readScanHeader(in_out_pSetjmpStreamState, in_setjmpStreamReadInterface, &in_pJpegContext->scanHeader);

	// TODO: There is a loop for this. Implement it
	Decode_restart_interval(in_out_pSetjmpStreamState, in_setjmpStreamReadInterface, 
		in_pJpegContext);
}

// E.2.4 Control procedure for decoding a restart interval
void Decode_restart_interval(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	const JpegContext *in_pcJpegContext)
{
	uint16_t currentMCUIndex = 0;

	Reset_decoder();

	do
	{
		currentMCUIndex++;
		Decode_MCU(in_pcJpegContext);
	} while (currentMCUIndex != in_pcJpegContext->restartIntervalState.restartInterval.Ri);
}

void Reset_decoder()
{
	// TODO
}

// E.2.5 Control procedure for decoding a minimum coded unit (MCU)
void Decode_MCU(const JpegContext *in_pcJpegContext)
{
	size_t N = 0;

Decode_MCU_loop_begin:
	N++;
	Decode_data_unit();
	// TODO: find out Nb
	//if (N != Nb)
	// goto Decode_MCU_loop_begin;
}

void Decode_data_unit()
{

}
