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
#include <cassert>
#include <cstring> // for memset
#include "JpegDecoder/JpegDecoderMarkers.h"
#include "JpegDecoder/JpegDecoder.h"
#include "JpegDecoder/JpegDecoderUtil.h"
#include "JpegDecoder/JpegSegments.h"
#include "JpegDecoder/JpegContext.h"

void Decoder_setup(JpegContext *in_pContext)
{
	memset(in_pContext->quantizationTablesState.isQuantizationTableInitialized, 0, 
		sizeof(in_pContext->quantizationTablesState.isQuantizationTableInitialized));
}

// E.2.1 Control procedure for decoding compressed image data
void Decode_image(FILE* jpegFile)
{
	unsigned char currentMarker;
	JpegContext context;

	currentMarker = readMarker(jpegFile);

	if (currentMarker != SOI_MARKER)
	{
		fprintf(stderr, "Expected SOI marker\n");
		exit(1);
	}

	// SOI is a standalone marker
	assert(isStandaloneMarker(currentMarker));

	Decoder_setup(&context);

	currentMarker = readMarker(jpegFile);

	bool restartIntervalFound = false;
	RestartInterval restartInterval;

	while (
		currentMarker != SOF_0_MARKER  && 
		currentMarker != SOF_1_MARKER  && 
		currentMarker != SOF_2_MARKER  && 
		currentMarker != SOF_3_MARKER  && 
		currentMarker != SOF_9_MARKER  && 
		currentMarker != SOF_10_MARKER && 
		currentMarker != SOF_11_MARKER &&
		currentMarker != SOF_15_MARKER &&
		currentMarker != SOF_55_MARKER)
	{
		switch (currentMarker)
		{
		case DRI_MARKER:    // 0xDD
			readRestartInterval(jpegFile, &restartInterval);
			restartIntervalFound = true;
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
			defaultMarkerInterpreter(jpegFile, currentMarker);
			break;
		default:
			fprintf(stderr, "Invalid marker in Decode_image. Exiting.\n");
			exit(1);
		}
		
		currentMarker = readMarker(jpegFile);
	}

	return Decode_frame(jpegFile, currentMarker, restartIntervalFound ? &restartInterval : NULL);
}

// E.2.2 Control procedure for decoding a frame
void Decode_frame(FILE* jpegFile, unsigned char currentMarker, RestartInterval* in_pri)
{
	// TODO: Replace by "Interpret frame header"
	defaultMarkerInterpreter(jpegFile, currentMarker);
	currentMarker = readMarker(jpegFile);

	bool restartIntervalFound = false;
	RestartInterval restartInterval;

	if (in_pri != NULL)
	{
		restartIntervalFound = true;
		restartInterval = *in_pri;
	}

	while (currentMarker != SOS_MARKER)
	{
		switch (currentMarker)
		{
		case DRI_MARKER:
			readRestartInterval(jpegFile, &restartInterval);
			restartIntervalFound = true;
			break;
		default:
			defaultMarkerInterpreter(jpegFile, currentMarker);
			break;
		}

		currentMarker = readMarker(jpegFile);
	}

	if (restartIntervalFound)
	{
		Decode_scan(jpegFile, restartInterval);
	}
	else
	{
		fprintf(stderr, "Trying to call Decode_scan but no restart interval was defined.\n");
		exit(1);
	}

	// TODO: further markers can occur before EOI
}

// E.2.3 Control procedure for decoding a scan
void Decode_scan(FILE* jpegFile, RestartInterval in_ri)
{
	ScanHeader sh;
	readScanHeader(jpegFile, &sh);

	size_t m = 0;

	// TODO: There is a loop for this. Implement it
	Decode_restart_interval(jpegFile, in_ri);
}

// E.2.4 Control procedure for decoding a restart interval
void Decode_restart_interval(FILE* jpegFile, RestartInterval in_ri)
{
	Reset_decoder();

	uint16_t currentMCUIndex = 0;

	do
	{
		currentMCUIndex++;
		Decode_MCU();
	} while (currentMCUIndex != in_ri.Ri);
}

void Reset_decoder()
{

}

// E.2.5 Control procedure for decoding a minimum coded unit (MCU)
void Decode_MCU()
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
