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

#ifndef _MTAx_RFC1950_zlib_h
#define _MTAx_RFC1950_zlib_h

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/declspec.h"

#include "IO/ByteStreamInterface.h"
#include "Util/ReadResult.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

/*!
 * Compare section
 * 2.2. Data format
 * of RFC 1950
 */
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4214)
#endif
typedef struct
{
	union
	{
		struct {
			uint8_t CM : 4;
			uint8_t CINFO : 4;
		} fields;
		uint8_t byteValue;
	} CMF;
	union
	{
		struct
		{
			uint8_t FCHECK : 5;
			uint8_t FDICT : 1;
			uint8_t FLEVEL : 2;
		} fields;
		uint8_t byteValue;
	} FLG;
} ZlibStreamHeader;
#ifdef _WIN32
#pragma warning(pop)
#endif

#pragma pack(pop)

DLLEXPORT ReadResult parseRFC1950(
	ByteStreamInterface *in_out_pReadStreamInterface, 
	ByteStreamInterface *in_out_pWriteStreamInterface, 
	bool in_supportPresetDictionary);

#ifdef __cplusplus
}
#endif

#endif
