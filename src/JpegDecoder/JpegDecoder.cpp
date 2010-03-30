#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoderMarkers.h"
#include "JpegDecoder/JpegDecoder.h"
#include "JpegDecoder/JpegDecoderUtil.h"
#include "JpegDecoder/JpegSegments.h"

void Decoder_setup()
{

}

void Decode_image(FILE* jpegFile)
{
	unsigned char currentMarker;

	currentMarker = readMarker(jpegFile);

	if (currentMarker != SOI_MARKER)
	{
		fprintf(stderr, "Expected SOI marker\n");
		exit(1);
	}

	// SOI is a standalone marker
	assert(isStandaloneMarker(currentMarker));

	Decoder_setup();

	currentMarker = readMarker(jpegFile);

	bool restartIntervalFound = false;
	RestartInterval restartInterval;

	// BUG: Not all of the values between define a SOF_n marker
	while (!(currentMarker >= SOF_0_MARKER && currentMarker <= SOF_15_MARKER))
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

	return Decode_frame(jpegFile, currentMarker, restartIntervalFound ? &restartInterval : NULL);
}

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

void Decode_scan(FILE* jpegFile, RestartInterval in_ri)
{
	defaultMarkerInterpreter(jpegFile, SOS_MARKER);

	size_t m = 0;

	// TODO: There is a loop for this. Implement it
	Decode_restart_interval(jpegFile, in_ri);
}

void Decode_restart_interval(FILE* jpegFile, RestartInterval in_ri)
{
	Reset_decoder();

	// a 32 bit variable can't overflow here since in_ri.Ri has 16 bits

	boost::uint16_t currentMCUIndex = 0;
Decode_restart_interval_loop_begin:
	currentMCUIndex++;
	Decode_MCU();
	if (currentMCUIndex != in_ri.Ri)
		goto Decode_restart_interval_loop_begin;
}

void Reset_decoder()
{

}

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
