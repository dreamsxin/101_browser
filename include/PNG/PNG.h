#ifndef _PNG_h
#define _PNG_h

#include "Util/ReadResult.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"

#pragma pack(push, 1)

/*
 * Compare section
 * 5.3 Chunk layout
 * of the PNG specification
 */
typedef struct
{
	uint32_t length;
	uint8_t chunkType[4];
} PNG_Chunk_Header;

typedef struct
{
	PNG_Chunk_Header header;
	uint8_t *chunkData;
	uint32_t crc;
} PNG_Chunk;

/*
 * See section
 * 11.2.2 IHDR Image header
 * of the PNG specification
 */
typedef struct
{
	uint32_t Width; // 4 bytes
	uint32_t Height; // 4 bytes
	uint8_t Bit_depth; // 1 byte
	uint8_t Colour_type; // 1 byte
	uint8_t Compression_method; // 1 byte
	uint8_t Filter_method; // 1 byte
	uint8_t Interlace_method; // 1 byte
} PNG_Chunk_Data_IHDR;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
ReadResult read_PNG(FILE* in_pngFile);

ReadResult read_PNG_Chunk_Header(PNG_Chunk_Header *out_pHeader, bool *out_isEndOfStream, FILE* in_pngFile);

ReadResult read_PNG_Chunk_Data_IHDR(PNG_Chunk_Data_IHDR *out_pChunkData, FILE* in_pngFile);

#ifdef __cplusplus
}
#endif

#endif