#ifndef _JpegSegments
#define _JpegSegments

#include <boost/cstdint.hpp>
#include <cstdio>
#include "JpegDecoder/JpegDecoderUtil.h"

// B.2.4.4 Restart interval definition syntax
struct RestartInterval
{
	boost::uint16_t Lr; // Define restart interval segment length
	boost::uint16_t Ri; // Specifies the number of MCU in the restart interval.
};

void readRestartInterval(FILE* jpegFile, RestartInterval* in_pRestartInterval);

#endif