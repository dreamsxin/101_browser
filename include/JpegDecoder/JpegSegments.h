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

#ifndef _JpegSegments
#define _JpegSegments

#include "BasicDataStructures/Types.h"
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
