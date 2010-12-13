#include "PNG/PNG.h"
#include <string.h>
#include <stddef.h> // for offsetof

// See http://www.w3.org/TR/PNG/#5PNG-file-signature
const uint8_t PNG_signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

ReadResult read_PNG(FILE* in_pngFile)
{
	uint8_t readPngSignature[8];

	if (fread(readPngSignature, 1, 8, in_pngFile) != 8)
		return ReadResultPrematureEndOfStream;

	if (memcmp(readPngSignature, PNG_signature, 8) != 0)
		return ReadResultInvalidData;

	return ReadResultOK;
}

ReadResult read_PNG_Chunk(PNG_Chunk *out_pPNG_Chunk, FILE* in_pngFile)
{
	if (fread(out_pPNG_Chunk, offsetof(PNG_Chunk, chunkData), 1, in_pngFile) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}
	
	return ReadResultNotImplemented;
}
