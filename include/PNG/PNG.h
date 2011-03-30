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

#ifndef _PNG_h
#define _PNG_h

#include "Util/ReadResult.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/declspec.h"

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

DLLEXPORT ReadResult read_PNG(FILE* in_pngFile);

ReadResult read_PNG_Chunk_Header(PNG_Chunk_Header *out_pHeader, 
	bool *out_isEndOfStream, FILE* in_pngFile, uint32_t *in_pCurrentCRC);

ReadResult read_PNG_Chunk_Data_and_CRC(const PNG_Chunk_Header *in_pHeader,
	void *in_out_pData, FILE* in_pngFile, uint32_t *in_pCurrentCRC,
	ReadResult (*in_pRead_PNG_Chunk_Data_Fun)(const PNG_Chunk_Header*, void *pData, FILE*, uint32_t*));

ReadResult read_PNG_Chunk_Data_Default(const PNG_Chunk_Header *in_pHeader, 
	void *_pData, FILE* in_pngFile, uint32_t *in_pCurrentCRC);

ReadResult read_PNG_Chunk_Data_IHDR(const PNG_Chunk_Header *in_pHeader, 
	void *out_pChunkDataIHDR, FILE* in_pngFile, uint32_t *in_pCurrentCRC);

ReadResult read_PNG_Chunk_Data_IDAT(const PNG_Chunk_Header *in_pHeader, 
	void *_pData, FILE* in_pngFile, uint32_t *in_pCurrentCRC);

#ifdef __cplusplus
}
#endif

#endif
