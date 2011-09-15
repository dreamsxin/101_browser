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

#ifndef _Ogg_Ogg_h
#define _Ogg_Ogg_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/declspec.h"
#include "IO/ByteStream.h"
#include "Util/ReadResult.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
	
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4214)
#endif

typedef struct
{
	uint8_t capture_pattern[4];
	uint8_t stream_structure_version;
	struct
	{
		uint8_t continuation : 1;
		uint8_t bos : 1;
		uint8_t eos : 1;
		uint8_t unused : 5;
	} header_type_flag;
	uint64_t granule_position;
	uint32_t bitstream_serial_number;
	uint32_t page_sequence_number;
	uint32_t CRC_checksum;
	uint8_t number_page_segments;
	uint8_t lacing_values[255];
} OggPageHeader;

#ifdef _WIN32
#pragma warning(pop)
#endif

#pragma pack(pop)

DLLEXPORT ReadResult readOgg(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface);

#ifdef __cplusplus
}
#endif

#endif
