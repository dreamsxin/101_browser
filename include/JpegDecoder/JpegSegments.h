#ifndef _JpegSegments
#define _JpegSegments

/*
 * Visual Studio 2010 is the first version to 
 * contains cstdint. In the versions before we
 * have to use boost
 */
#if defined(_MSC_VER) && (_MSC_VER<=1500)
#include "boost/cstdint.hpp"
using namespace boost; // This is a hack - you should not use this in header files
#else
#include <cstdint>
#endif
#include <cstdio>
#include <vector>
#include "JpegDecoder/JpegDecoderUtil.h"

// B.2.4.4 Restart interval definition syntax
struct RestartInterval
{
	uint16_t Lr; // Define restart interval segment length
	uint16_t Ri; // Specifies the number of MCU in the restart interval.
};

void readRestartInterval(FILE* jpegFile, RestartInterval* in_pRestartInterval);

#pragma pack(push, 1)
// B.2.3 Scan header syntax
struct ScanHeader
{
	uint16_t Ls; // Scan header length – Specifies the length of the 
	                    // scan header shown in Figure B.4 (see B.1.1.4).
	uint8_t Ns;  // Number of image components in scan – Specifies 
	                    // the number of source image components in the scan. 
	                    // The value of Ns shall be equal to the number of sets
	                    // of scan component specification parameters 
	                    // (Csj, Tdj, and Taj) present in the scan header.

	struct ScanComponentSpecificationParameter
	{
		uint8_t Cs;
		// Note that the order is changed from the specification since
		// X86 seems to use little-endian bit ordering
		uint8_t Ta : 4;
		uint8_t Td : 4;
	};

	std::vector<ScanComponentSpecificationParameter> componentSpecificationParameters;

	uint8_t Ss;
	uint8_t Se;
	// Note that the order is changed from the specification since
	// X86 seems to use little-endian bit ordering
	uint8_t Al : 4;
	uint8_t Ah : 4;
};
#pragma pack(pop)

void readScanHeader(FILE* jpegFile, ScanHeader* in_pScanHeader);

#endif