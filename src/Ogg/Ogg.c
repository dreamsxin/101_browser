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
#include "MiniStdlib/cstring.h"      // for memcmp
#include "MiniStdlib/MTAx_cstdlib.h" // for malloc
#include "MiniStdlib/MTAx_cstdio.h"  // for printf
#include "MiniStdlib/safe_free.h"
#include "Algorithm/BinarySearch.h"

const char OggS[4] = { 'O', 'g', 'g', 'S' };

ReadResult readAndCheckOggPageHeader(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, OggPageHeader *out_pOggPageHeader, 
	uint32_t *out_pCrc)
{
	size_t readCount;
	
	*out_pCrc = CRC_init(false);

	readCount = in_readInterface.mpfRead(in_out_pReadStreamState, out_pOggPageHeader, 
		offsetof(OggPageHeader, lacing_values));
	
	if (0 == readCount)
		return ReadResultNullData;
	else if (readCount != offsetof(OggPageHeader, lacing_values))
		return ReadResultPrematureEndOfStream;

	if (memcmp(out_pOggPageHeader->capture_pattern, OggS, 4))
		return ReadResultInvalidData;

	if (out_pOggPageHeader->stream_structure_version != 0)
		return ReadResultNotImplemented;

	if (out_pOggPageHeader->header_type_flag.unused)
		return ReadResultInvalidData;

	if (out_pOggPageHeader->header_type_flag.bos && 
		out_pOggPageHeader->header_type_flag.continuation)
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

typedef enum
{
	OggBitstreamTypeUnknown,
	OggBitstreamTypeTheora,
	OggBitstreamTypeVorbis,
	OggBitstreamTypeSkeleton
} OggBitstreamType;

typedef struct
{
	uint32_t bitstream_serial_number;
	uint32_t page_sequence_number;
	OggBitstreamType oggBitstreamType;
} OggBitstreamState;

void initOggBitstreamState(OggBitstreamState *out_pOggBitstreamState, uint32_t in_serial_number, 
	OggBitstreamType in_oggBitstreamType)
{
	out_pOggBitstreamState->bitstream_serial_number = in_serial_number;
	out_pOggBitstreamState->page_sequence_number = 0;
	out_pOggBitstreamState->oggBitstreamType = in_oggBitstreamType;
}

IntermediateCompareResult oggBitStreamStateSearch(const void *in_pSerialNumber, 
	const void *in_pOggBitstreamState)
{
	uint32_t serialNumber = *(uint32_t*) in_pSerialNumber;
	OggBitstreamState *pOggBitstreamState = (OggBitstreamState*) in_pOggBitstreamState;
	
	if (serialNumber < pOggBitstreamState->bitstream_serial_number)
		return IntermediateCompareResultLess;
	else if (serialNumber > pOggBitstreamState->bitstream_serial_number)
		return IntermediateCompareResultGreater;
	else
		return IntermediateCompareResultEqual;
}

ReadResult readOgg(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface)
{
	OggPageHeader oggPageHeader;
	ReadResult readResult;
	uint32_t crc;

	size_t bitstreamStatesCount = 0;
	OggBitstreamState *pBitstreamStates = NULL;
	// Tells us whether the current block is the first block
	bool firstBlock = true;

	do
	{
		uint8_t currentPageSegment;
		size_t index;

		readResult = readAndCheckOggPageHeader(in_out_pReadStreamState, 
			in_readInterface, &oggPageHeader, &crc);

		if (firstBlock && ReadResultNullData == readResult)
			return ReadResultOK;
		else if (readResult != ReadResultOK)
			return readResult;

		if (!binarySearch(&oggPageHeader.bitstream_serial_number, pBitstreamStates, 
			bitstreamStatesCount, sizeof(OggBitstreamState), 
			oggBitStreamStateSearch, &index))
		{
			OggBitstreamState *newStates;

			if (!oggPageHeader.header_type_flag.bos)
				return ReadResultInvalidData;

			if (oggPageHeader.header_type_flag.continuation)
				return ReadResultInvalidData;

			if (oggPageHeader.page_sequence_number != 0)
				return ReadResultInvalidData;

			if (!oggPageHeader.header_type_flag.eos)
			{
				newStates = (OggBitstreamState*) realloc(pBitstreamStates, (bitstreamStatesCount+1)*sizeof(OggBitstreamState));
				
				if (!newStates)
				{
					safe_free(&pBitstreamStates);
					return ReadResultAllocationFailure;
				}
				
				pBitstreamStates = newStates;
				memmove(pBitstreamStates+index+1, pBitstreamStates+index, 
					(bitstreamStatesCount-index)*sizeof(OggBitstreamState));
				initOggBitstreamState(pBitstreamStates+index, oggPageHeader.bitstream_serial_number, 
					OggBitstreamTypeUnknown);
				bitstreamStatesCount++;
			}
		}
		else
		{
			if (oggPageHeader.page_sequence_number != pBitstreamStates[index].page_sequence_number+1)
				return ReadResultInvalidData;

			pBitstreamStates[index].page_sequence_number++;

			if (oggPageHeader.header_type_flag.eos)
			{
				OggBitstreamState *newStates;

				memmove(pBitstreamStates+index, pBitstreamStates+index+1, 
					(bitstreamStatesCount-index-1)*sizeof(OggBitstreamState));

				newStates = (OggBitstreamState*) realloc(pBitstreamStates, (bitstreamStatesCount-1)*sizeof(OggBitstreamState));

				/*
				* We test whether the allocation failed.
				* If we tried to allocate 0 bytes NULL may be returned - but must not otherwise
				* (since the new block is smaller than the old one).
				*
				* Q: Why 1 and not 0?
				* A: Because bitstreamStatesCount won't be decremented 
				* until further below.
				*/
				if (!newStates && bitstreamStatesCount > 1)
				{
					safe_free(&pBitstreamStates);
					return ReadResultInternalInconsistency;
				}

				pBitstreamStates = newStates;
				
				bitstreamStatesCount--;
			}
		}

		for (currentPageSegment = 0; currentPageSegment < oggPageHeader.number_page_segments; 
			currentPageSegment++)
		{
			uint8_t byte;
			uint8_t idx;

			for (idx = 0; idx < oggPageHeader.lacing_values[currentPageSegment]; idx++)
			{
				if (in_readInterface.mpfRead(in_out_pReadStreamState, &byte, 1) != 1)
				{
					safe_free(&pBitstreamStates);
					return ReadResultPrematureEndOfStream;
				}
				
				crc = CRC_foldl_MSB_to_LSB(crc, &byte, 1);
			}

			if (currentPageSegment == oggPageHeader.number_page_segments - 1)
			{
				if (crc != oggPageHeader.CRC_checksum)
				{
					safe_free(&pBitstreamStates);
					return ReadResultInvalidData;
				}
			}
		}

		firstBlock = false;
	} while (bitstreamStatesCount != 0);

#if 0
	if (oggPageHeader.number_page_segments == 0x01)
	{
		if (oggPageHeader.lacing_values[0] == sizeof(FisheadIdentHeader30) ||
			oggPageHeader.lacing_values[0] == sizeof(FisheadIdentHeader40))
		{
			FisheadIdentHeader40 fisheadIdentHeader;

			if (in_readInterface.mpfRead(in_out_pReadStreamState, &fisheadIdentHeader, 
				oggPageHeader.lacing_values[0]) != oggPageHeader.lacing_values[0])
				return ReadResultPrematureEndOfStream;

			if (memcmp(fisheadIdentHeader.fisheadIdentHeader30.Identifier, "fishead", 8))
				return ReadResultNotImplemented;

			if (oggPageHeader.lacing_values[0] == sizeof(FisheadIdentHeader30) && 
				(fisheadIdentHeader.fisheadIdentHeader30.Version_major != 0x3 || 
				fisheadIdentHeader.fisheadIdentHeader30.Version_minor != 0x0))
			{
				return ReadResultNotImplemented;
			}
			else if (oggPageHeader.lacing_values[0] == sizeof(FisheadIdentHeader40) && 
				(fisheadIdentHeader.fisheadIdentHeader30.Version_major != 0x4 || 
				fisheadIdentHeader.fisheadIdentHeader30.Version_minor != 0x0))
			{
				return ReadResultNotImplemented;
			}

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
#endif

	safe_free(&pBitstreamStates);
	return ReadResultOK;
}
