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

#include "PNG/PNG.h"
#include <string.h> // for memcmp
#include "MiniStdlib/MTAx_cstdlib.h" // for the conversation functions for endianness
#include "PNG/CRC.h"
#include "IO/fread.h"
#include "RFC1950/RFC1950_zlib.h"

// See http://www.w3.org/TR/PNG/#5PNG-file-signature
const uint8_t PNG_signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

ReadResult read_PNG(FILE* in_pngFile)
{
	uint8_t readPngSignature[8];
	PNG_Chunk_Header pngChunkHeader;
	bool isEndOfStream;
	ReadResult readResult;
	PNG_Chunk_Data_IHDR pngChunkDataIHDR;
	uint32_t readChecksum;

	/*
	* Meaning of this variable:
	* read_IDAT_State == 0: before reading IDAT chunk
	* read_IDAT_State == 1: having read/reading at least one IDAT, but not seen
	*                       another following chunk of not IDAT type
	* read_IDAT_State == 2: having read IDAT chunk(s) and read/reading another 
	*                       chunk after
	*/
	uint8_t read_IDAT_State = 0;

	if (fread(readPngSignature, 1, 8, in_pngFile) != 8)
		return ReadResultPrematureEndOfStream;

	if (memcmp(readPngSignature, PNG_signature, 8) != 0)
		return ReadResultInvalidData;

	readChecksum = CRC_init();
	readResult = read_PNG_Chunk_Header(&pngChunkHeader, &isEndOfStream, in_pngFile, &readChecksum);
	if (readResult != ReadResultOK)
		return readResult;
	if (isEndOfStream)
		return readResult;

	if (memcmp("IHDR", pngChunkHeader.chunkType, 4) != 0)
	{
		return ReadResultInvalidData;
	}

	if ((readResult = read_PNG_Chunk_Data_and_CRC(&pngChunkHeader, &pngChunkDataIHDR, in_pngFile, &readChecksum, 
		&read_PNG_Chunk_Data_IHDR)) != ReadResultOK)
		return readResult;

	while (1)
	{
		readChecksum = CRC_init();
		readResult = read_PNG_Chunk_Header(&pngChunkHeader, &isEndOfStream, in_pngFile, &readChecksum);

		if (readResult != ReadResultOK)
			return readResult;
		if (isEndOfStream)
			return readResult;

		{
			int IDAT_compareResult = memcmp(pngChunkHeader.chunkType, "IDAT", 4);

			if (0 == IDAT_compareResult && 0 == read_IDAT_State)
			{
				read_IDAT_State = 1;
			}
			else if (0 != IDAT_compareResult && 1 == read_IDAT_State)
			{
				read_IDAT_State = 2;
			}
		}

		printf("%c%c%c%c: %u\n", 
			pngChunkHeader.chunkType[0], 
			pngChunkHeader.chunkType[1], 
			pngChunkHeader.chunkType[2], 
			pngChunkHeader.chunkType[3], 
			pngChunkHeader.length);

		if (0 == memcmp(pngChunkHeader.chunkType, "gAMA", 4))
		{
			if (read_IDAT_State == 0)
			{
				if ((readResult = read_PNG_Chunk_Data_and_CRC(&pngChunkHeader, &pngChunkDataIHDR, in_pngFile, &readChecksum, 
					&read_PNG_Chunk_Data_Default)) != ReadResultOK)
					return readResult;
			}
			else
			{
				return ReadResultInvalidData;
			}
		}
		else if (0 == memcmp(pngChunkHeader.chunkType, "IDAT", 4))
		{
			if (1 != read_IDAT_State)
				return ReadResultInvalidData;

			if ((readResult = read_PNG_Chunk_Data_and_CRC(&pngChunkHeader, &pngChunkDataIHDR, in_pngFile, &readChecksum, 
				&read_PNG_Chunk_Data_IDAT)) != ReadResultOK)
				return readResult;
		}
		else
		{
			if ((readResult = read_PNG_Chunk_Data_and_CRC(&pngChunkHeader, &pngChunkDataIHDR, in_pngFile, &readChecksum, 
				&read_PNG_Chunk_Data_Default)) != ReadResultOK)
				return readResult;
		}
	}

	return ReadResultOK;
}

ReadResult read_PNG_Chunk_Header(PNG_Chunk_Header *out_pHeader, 
	bool *out_isEndOfStream, FILE* in_pngFile, uint32_t *in_pCurrentCRC)
{
	size_t readHeaderBytes = fread(&out_pHeader->length, 1, sizeof(out_pHeader->length), in_pngFile);

	if (0 == readHeaderBytes)
	{
		*out_isEndOfStream = true;
		return ReadResultOK;
	}
	else if (sizeof(out_pHeader->length) != readHeaderBytes)
	{
		*out_isEndOfStream = true;
		return ReadResultPrematureEndOfStream;
	}

	out_pHeader->length = _byteswap_ulong(out_pHeader->length);

	readHeaderBytes = fread_withState(&out_pHeader->chunkType, 1, sizeof(out_pHeader->chunkType), in_pngFile, 
		in_pCurrentCRC, &CRC_stateUpdate);

	if (sizeof(out_pHeader->chunkType) != readHeaderBytes)
	{
		*out_isEndOfStream = true;
		return ReadResultPrematureEndOfStream;
	}

	*out_isEndOfStream = false;

	return ReadResultOK;
}

ReadResult read_PNG_Chunk_Data_and_CRC(const PNG_Chunk_Header *in_pHeader,
	void *in_out_pData, FILE* in_pngFile, uint32_t *in_pCurrentCRC,
	ReadResult (*in_pRead_PNG_Chunk_Data_Fun)(const PNG_Chunk_Header*, void*, FILE*, uint32_t*))
{
	ReadResult readResult;
	uint32_t chunkCRC;

	if ((readResult = (*in_pRead_PNG_Chunk_Data_Fun)(in_pHeader, in_out_pData, in_pngFile, in_pCurrentCRC)) 
		!= ReadResultOK)
		return readResult;

	*in_pCurrentCRC = CRC_terminate(*in_pCurrentCRC);

	if (fread(&chunkCRC, sizeof(chunkCRC), 1, in_pngFile) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}

	chunkCRC = _byteswap_ulong(chunkCRC);

	if (chunkCRC != *in_pCurrentCRC)
		return ReadResultInvalidData;

	return ReadResultOK;
}

ReadResult read_PNG_Chunk_Data_Default(const PNG_Chunk_Header *in_pHeader, 
	void *_pData, FILE* in_pngFile, uint32_t *in_pCurrentCRC)
{
	uint64_t index64;

	for (index64 = 0; index64 < in_pHeader->length; index64++)
	{
		uint8_t aByte;

		if (fread(&aByte, 1, 1, in_pngFile) != 1)
			return ReadResultPrematureEndOfStream;

		*in_pCurrentCRC = CRC_update(*in_pCurrentCRC, aByte);
	}

	return ReadResultOK;
}

ReadResult read_PNG_Chunk_Data_IHDR(const PNG_Chunk_Header *in_pHeader, 
	void *out_pChunkDataIHDR, FILE* in_pngFile, uint32_t *in_pCurrentCRC)
{
	PNG_Chunk_Data_IHDR *pChunkDataIHDR = (PNG_Chunk_Data_IHDR *) out_pChunkDataIHDR;

	if (sizeof(PNG_Chunk_Data_IHDR) != in_pHeader->length)
	{
		return ReadResultInvalidData;
	}

	if (fread_withState(pChunkDataIHDR, sizeof(*pChunkDataIHDR), 1, in_pngFile, in_pCurrentCRC, &CRC_stateUpdate) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}

	pChunkDataIHDR->Width = _byteswap_ulong(pChunkDataIHDR->Width);
	pChunkDataIHDR->Height = _byteswap_ulong(pChunkDataIHDR->Height);

	// See Table 11.1 — Allowed combinations of colour type and bit depth
	switch (pChunkDataIHDR->Colour_type)
	{
	case 0:
		switch (pChunkDataIHDR->Bit_depth)
		{
		case 1:
		case 2:
		case 4:
		case 8:
		case 16:
			break;
		default:
			return ReadResultInvalidData;
		}
		break;
	case 2:
		switch (pChunkDataIHDR->Bit_depth)
		{
		case 8:
		case 16:
			break;
		default:
			return ReadResultInvalidData;
		}
		break;
	case 3:
		switch (pChunkDataIHDR->Bit_depth)
		{
		case 1:
		case 2:
		case 4:
		case 8:
			break;
		default:
			return ReadResultInvalidData;
		}
		break;
	case 4:
	case 6:
		switch (pChunkDataIHDR->Bit_depth)
		{
		case 8:
		case 16:
			break;
		default:
			return ReadResultInvalidData;
		}
		break;
	default:
		return ReadResultInvalidData;
	}

	return ReadResultOK;
}

ReadResult read_PNG_Chunk_Data_IDAT(const PNG_Chunk_Header *in_pHeader, 
	void *_pData, FILE* in_pngFile, uint32_t *in_pCurrentCRC)
{
	uint64_t index64;

	for (index64 = 0; index64 < in_pHeader->length; index64++)
	{
		uint8_t aByte;

		if (fread(&aByte, 1, 1, in_pngFile) != 1)
			return ReadResultPrematureEndOfStream;

		*in_pCurrentCRC = CRC_update(*in_pCurrentCRC, aByte);
	}

	return ReadResultOK;
}
