#include "PNG/PNG.h"
#include <string.h>
#include <stddef.h> // for offsetof
#include "MiniStdlib/MTAx_cstdlib.h" // for the conversation functions for endianness

// See http://www.w3.org/TR/PNG/#5PNG-file-signature
const uint8_t PNG_signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

ReadResult read_PNG(FILE* in_pngFile)
{
	uint8_t readPngSignature[8];
	PNG_Chunk pngChunk;
	size_t readHeaderBytes;

	if (fread(readPngSignature, 1, 8, in_pngFile) != 8)
		return ReadResultPrematureEndOfStream;

	if (memcmp(readPngSignature, PNG_signature, 8) != 0)
		return ReadResultInvalidData;

	while (1)
	{
		uint64_t index64;

		readHeaderBytes = fread(&pngChunk.header, 1, sizeof(pngChunk.header), in_pngFile);

		if (0 == readHeaderBytes)
		{
			return ReadResultOK;
		}
		else if (sizeof(pngChunk.header) != readHeaderBytes)
		{
			return ReadResultPrematureEndOfStream;
		}

		pngChunk.header.length = _byteswap_ulong(pngChunk.header.length);

		printf("%c%c%c%c: %u\n", 
			pngChunk.header.chunkType[0], 
			pngChunk.header.chunkType[1], 
			pngChunk.header.chunkType[2], 
			pngChunk.header.chunkType[3], 
			pngChunk.header.length);

		for (index64 = 0; index64 < pngChunk.header.length; index64++)
		{
			uint8_t aByte;

			if (fread(&aByte, 1, 1, in_pngFile) != 1)
				return ReadResultPrematureEndOfStream;
		}

		if (fread(&pngChunk.crc, 4, 1, in_pngFile) != 1)
			return ReadResultPrematureEndOfStream;
	}

	return ReadResultOK;
}
