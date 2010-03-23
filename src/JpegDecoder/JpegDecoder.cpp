#include <cstdlib>
#include <cassert>
#include "JpegDecoder/JpegDecoderMarkers.h"
#include "JpegDecoder/JpegDecoder.h"
#include "JpegDecoder/JpegDecoderUtil.h"

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

	// BUG: Not all of the values between define a SOF_n marker
	while (!(currentMarker >= SOF_0_MARKER && currentMarker <= SOF_15_MARKER))
	{
		defaultMarkerInterpreter(jpegFile, currentMarker);
		currentMarker = readMarker(jpegFile);
	}

	return Decode_frame(jpegFile, currentMarker);
}

void Decode_frame(FILE* jpegFile, unsigned char currentMarker)
{
	// TODO: Replace by "Interpret frame header"
	defaultMarkerInterpreter(jpegFile, currentMarker);
	currentMarker = readMarker(jpegFile);

	while (currentMarker != SOS_MARKER)
	{
		defaultMarkerInterpreter(jpegFile, currentMarker);
		currentMarker = readMarker(jpegFile);
	}

	Decode_scan(jpegFile);

	// TODO: further markers can occur before EOI
}

void Decode_scan(FILE* jpegFile)
{
	defaultMarkerInterpreter(jpegFile, SOS_MARKER);

	size_t m = 0;

	Decode_restart_interval();
}

void Decode_restart_interval()
{

}