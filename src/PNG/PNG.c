#include "PNG/PNG.h"
#include <string.h>
#include <stddef.h> // for offsetof
#include "Algorithm/Endianness.h"

// See http://www.w3.org/TR/PNG/#5PNG-file-signature
const uint8_t PNG_signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

ReadResult read_PNG(FILE* in_pngFile)
{
	uint8_t readPngSignature[8];
	PNG_Chunk pngChunk;

	if (fread(readPngSignature, 1, 8, in_pngFile) != 8)
		return ReadResultPrematureEndOfStream;

	if (memcmp(readPngSignature, PNG_signature, 8) != 0)
		return ReadResultInvalidData;

	while (read_PNG_Chunk(&pngChunk, in_pngFile) != ReadResultPrematureEndOfStream)
	{
		printf("%c%c%c%c: %u\n", pngChunk.chunkType[0], pngChunk.chunkType[1], pngChunk.chunkType[2], pngChunk.chunkType[3], pngChunk.length);
	}

	return ReadResultOK;
}

ReadResult read_PNG_Chunk(PNG_Chunk *out_pPNG_Chunk, FILE* in_pngFile)
{
	if (fread(out_pPNG_Chunk, offsetof(PNG_Chunk, chunkData), 1, in_pngFile) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}
	
	flipEndianness(&out_pPNG_Chunk->length, sizeof(out_pPNG_Chunk->length));

	// this is not a good idea...
	if (fseek(in_pngFile, out_pPNG_Chunk->length, SEEK_CUR) != 0)
		return ReadResultPrematureEndOfStream;

	if (fread(&out_pPNG_Chunk->crc, sizeof(out_pPNG_Chunk->crc), 1, in_pngFile) != 1)
		return ReadResultPrematureEndOfStream;

	return ReadResultOK;
}
