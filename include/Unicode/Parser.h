/*
* Copyright 2008-2012 Wolfgang Keller
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

#ifndef _Unicode_Parser_h
#define _Unicode_Parser_h

#include "Unicode/Unicode.h"
#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"
#include "IO/ByteStream.h"
#include "IO/ByteStream_v3.h"
#include "Util/ReadResult.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	ByteStreamReadInterface_v3 readInterface;
	void *pReadState;
} ParserState;

typedef struct
{
	// Permanent settings
	ParserState parserState;
	bool bigEndian;

	// Temporary variables
	UnicodeCodePoint currentCodePoint;
	bool isSecondByte;
	uint16_t currentWord;
} UTF16_State;

typedef struct
{
	// Permanent settings
	ParserState parserState;
	bool bigEndian;
} UTF32_State;

DLLEXPORT ReadResult parse_UTF8(
	ByteStreamInterface in_readInterface, 
	void *in_pReadState,
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState);

DLLEXPORT ReadResult parse_UTF16(
	ByteStreamReadInterface_v3 in_readInterface, 
	void *in_pReadState,
	ByteStreamWriteInterface_v3 in_writeInterface,
	void *in_pWriteState, 
	bool in_bigEndian);

DLLEXPORT void utf32_StateInit(UTF32_State *out_pState, 
	ByteStreamReadInterface_v3 in_readInterface, 
	void *in_pReadState, 
	bool in_bigEndian);

DLLEXPORT void utf32_StateReset(UTF32_State *out_pState);

DLLEXPORT ByteStreamReadInterface_v3 getUTF32_ReadInterface();

/*!
* Parameters:
* in_count: number of codepoints (not bytes!) to read
*/
DLLEXPORT size_t utf32_read(
	void *in_out_pByteStreamState, 
	void *out_pBuffer, size_t in_count);

#ifdef __cplusplus
}
#endif

#endif
