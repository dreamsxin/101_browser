/*
* Copyright 2011 Wolfgang Keller
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

#include "Ogg/Ogg.h"
#include "Ogg/Skeleton.h"
#include "CRC/CRC.h"
#include "MiniStdlib/cstring.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "Algorithm/BinarySearch.h"

const char OggS[4] = { 'O', 'g', 'g', 'S' };

ReadResult readAndCheckOggPageHeader(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, OggPageHeader *out_pOggPageHeader, 
	uint32_t *out_pCrc)
{
	*out_pCrc = CRC_init(false);
	
	if (in_readInterface.mpfRead(in_out_pReadStreamState, out_pOggPageHeader, 
		offsetof(OggPageHeader, lacing_values)) 
		!= offsetof(OggPageHeader, lacing_values))
		return ReadResultPrematureEndOfStream;

	if (memcmp(out_pOggPageHeader->capture_pattern, OggS, 4))
		return ReadResultInvalidData;

	if (out_pOggPageHeader->stream_structure_version != 0)
		return ReadResultNotImplemented;

	if (out_pOggPageHeader->header_type_flag.unused)
		return ReadResultInvalidData;

	*out_pCrc = CRC_foldl_MSB_to_LSB(*out_pCrc, out_pOggPageHeader, 
		offsetof(OggPageHeader, CRC_checksum));
	*out_pCrc = CRC_update_MSB_to_LSB(*out_pCrc, 0);
	*out_pCrc = CRC_update_MSB_to_LSB(*out_pCrc, 0);
	*out_pCrc = CRC_update_MSB_to_LSB(*out_pCrc, 0);
	*out_pCrc = CRC_update_MSB_to_LSB(*out_pCrc, 0);

	if (in_readInterface.mpfRead(in_out_pReadStreamState, 
		out_pOggPageHeader->lacing_values, 
		out_pOggPageHeader->number_page_segments) != 
		out_pOggPageHeader->number_page_segments)
	{
		return ReadResultPrematureEndOfStream;
	}

	*out_pCrc = CRC_foldl_MSB_to_LSB(*out_pCrc, &out_pOggPageHeader->number_page_segments, 
		1+out_pOggPageHeader->number_page_segments);

	return ReadResultOK;
}

typedef struct
{
	uint32_t bitstream_serial_number;
} OggBitstreamState;

ReadResult readOgg(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface)
{
	OggPageHeader oggPageHeader;
	ReadResult readResult;
	uint32_t crc;

	OggBitstreamState *pBitstreamStates = NULL;

	if ((readResult = readAndCheckOggPageHeader(in_out_pReadStreamState, 
		in_readInterface, &oggPageHeader, &crc)) != ReadResultOK)
		return readResult;
	
	printf("Serial number: %x\n", oggPageHeader.bitstream_serial_number);

	if (!oggPageHeader.header_type_flag.bos)
		return ReadResultInvalidData;

	if (oggPageHeader.number_page_segments == 0x01)
	{
		if (oggPageHeader.lacing_values[0] == 0x40)
		{
			FisheadIdentHeader fisheadIdentHeader;

			if (in_readInterface.mpfRead(in_out_pReadStreamState, &fisheadIdentHeader, 
				sizeof(FisheadIdentHeader)) != sizeof(FisheadIdentHeader))
				return ReadResultPrematureEndOfStream;

			if (memcmp(fisheadIdentHeader.Identifier, "fishead", 8))
				return ReadResultNotImplemented;

			if (fisheadIdentHeader.Version_major != 0x3 ||
				fisheadIdentHeader.Version_minor != 0x0)
				return ReadResultNotImplemented;

			crc = CRC_foldl_MSB_to_LSB(crc, &fisheadIdentHeader, 
				oggPageHeader.lacing_values[0]);

			if (crc != oggPageHeader.CRC_checksum)
				return ReadResultInvalidData;
		}
		else
			return ReadResultNotImplemented;
	}
	else
		return ReadResultNotImplemented;

	while (!oggPageHeader.header_type_flag.eos)
	{
		uint8_t currentPageSegment;
		
		if ((readResult = readAndCheckOggPageHeader(in_out_pReadStreamState, 
			in_readInterface, &oggPageHeader, &crc)) != ReadResultOK)
			return readResult;
		
		printf("Serial number: %x\n", oggPageHeader.bitstream_serial_number);

		for (currentPageSegment = 0; currentPageSegment < oggPageHeader.number_page_segments; 
			currentPageSegment++)
		{
			uint8_t byte;
			uint8_t idx;

			for (idx = 0; idx < oggPageHeader.lacing_values[currentPageSegment]; idx++)
			{
				if (in_readInterface.mpfRead(in_out_pReadStreamState, &byte, 1) != 1)
					return ReadResultPrematureEndOfStream;
				
				crc = CRC_foldl_MSB_to_LSB(crc, &byte, 1);
			}

			if (currentPageSegment == oggPageHeader.number_page_segments - 1)
			{
				if (crc != oggPageHeader.CRC_checksum)
					return ReadResultInvalidData;
			}
		}
	}

	return ReadResultOK;
}
