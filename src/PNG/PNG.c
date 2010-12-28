#include "PNG/PNG.h"
#include <string.h> // for memcmp
#include "MiniStdlib/MTAx_cstdlib.h" // for the conversation functions for endianness
#include "PNG/CRC.h"
#include "IO/fread.h"

// See http://www.w3.org/TR/PNG/#5PNG-file-signature
const uint8_t PNG_signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

ReadResult read_PNG(FILE* in_pngFile)
{
	uint8_t readPngSignature[8];
	PNG_Chunk pngChunk;
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
	readResult = read_PNG_Chunk_Header(&pngChunk.header, &isEndOfStream, in_pngFile, &readChecksum);
	if (readResult != ReadResultOK)
		return readResult;
	if (isEndOfStream)
		return readResult;

	if (memcmp("IHDR", pngChunk.header.chunkType, 4) != 0)
	{
		return ReadResultInvalidData;
	}

	if (sizeof(PNG_Chunk_Data_IHDR) != pngChunk.header.length)
	{
		return ReadResultInvalidData;
	}

	if ((readResult = read_PNG_Chunk_Data_IHDR(&pngChunkDataIHDR, in_pngFile, &readChecksum)) != ReadResultOK)
		return readResult;

	readChecksum = CRC_terminate(readChecksum);

	if (fread(&pngChunk.crc, sizeof(pngChunk.crc), 1, in_pngFile) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}

	pngChunk.crc = _byteswap_ulong(pngChunk.crc);

	if (pngChunk.crc != readChecksum)
		return ReadResultInvalidData;

	while (1)
	{
		readChecksum = CRC_init();
		readResult = read_PNG_Chunk_Header(&pngChunk.header, &isEndOfStream, in_pngFile, &readChecksum);

		if (readResult != ReadResultOK)
			return readResult;
		if (isEndOfStream)
			return readResult;

		{
			int IDAT_compareResult = memcmp(pngChunk.header.chunkType, "IDAT", 4);

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
			pngChunk.header.chunkType[0], 
			pngChunk.header.chunkType[1], 
			pngChunk.header.chunkType[2], 
			pngChunk.header.chunkType[3], 
			pngChunk.header.length);

		if (0 == memcmp(pngChunk.header.chunkType, "gAMA", 4))
		{
			if (read_IDAT_State == 0)
			{
				read_PNG_Chunk_Data_Default(&pngChunk.header, in_pngFile, &readChecksum);
			}
			else
			{
				return ReadResultInvalidData;
			}
		}
		else if (0 == memcmp(pngChunk.header.chunkType, "IDAT", 4))
		{
			if (1 != read_IDAT_State)
				return ReadResultInvalidData;
			else
				read_PNG_Chunk_Data_Default(&pngChunk.header, in_pngFile, &readChecksum);
		}
		else
		{
			read_PNG_Chunk_Data_Default(&pngChunk.header, in_pngFile, &readChecksum);
		}

		readChecksum = CRC_terminate(readChecksum);

		if (fread(&pngChunk.crc, 4, 1, in_pngFile) != 1)
			return ReadResultPrematureEndOfStream;

		pngChunk.crc = _byteswap_ulong(pngChunk.crc);

		if (pngChunk.crc != readChecksum)
			return ReadResultInvalidData;
	}

	return ReadResultOK;
}

ReadResult read_PNG_Chunk_Header(PNG_Chunk_Header *out_pHeader, bool *out_isEndOfStream, FILE* in_pngFile, 
	uint32_t *in_pCurrentCRC)
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

ReadResult read_PNG_Chunk_Data_Default(const PNG_Chunk_Header *in_pHeader, FILE* in_pngFile, uint32_t *in_pCurrentCRC)
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

ReadResult read_PNG_Chunk_Data_IHDR(PNG_Chunk_Data_IHDR *out_pChunkData, FILE* in_pngFile, uint32_t *in_pCurrentCRC)
{
	if (fread_withState(out_pChunkData, sizeof(*out_pChunkData), 1, in_pngFile, in_pCurrentCRC, &CRC_stateUpdate) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}

	out_pChunkData->Width = _byteswap_ulong(out_pChunkData->Width);
	out_pChunkData->Height = _byteswap_ulong(out_pChunkData->Height);

	// See Table 11.1 — Allowed combinations of colour type and bit depth
	switch (out_pChunkData->Colour_type)
	{
	case 0:
		switch (out_pChunkData->Bit_depth)
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
		switch (out_pChunkData->Bit_depth)
		{
		case 8:
		case 16:
			break;
		default:
			return ReadResultInvalidData;
		}
		break;
	case 3:
		switch (out_pChunkData->Bit_depth)
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
		switch (out_pChunkData->Bit_depth)
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
