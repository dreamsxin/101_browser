/*
* Copyright 2012 Wolfgang Keller
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

#ifndef _Unicode_Parser_UTF32_h
#define _Unicode_Parser_UTF32_h

#include "Unicode/Unicode.h"
#include "Unicode/Parser.h"
#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"
#include "IO/ByteStream_v4.h"

#ifdef __cplusplus
extern "C" {
#endif

// UTF-32 data structures
typedef enum
{
	UTF32_EntryPoint_BeforeReading,
	UTF32_EntryPoint_WriteTerminalReplacementCharacter,
	UTF32_EntryPoint_WriteCharacter,
	UTF32_EntryPoint_Terminated
} UTF32_EntryPoint;

typedef struct
{
	// Permanent settings
	bool bigEndian;

	// Temporary variables
	uint8_t readCount;
	UTF32_EntryPoint entryPoint;
	UnicodeCodePoint currentCodePoint;
} UTF32_State;

// UTF-32 functions
DLLEXPORT void utf32_StateInit(UTF32_State *out_pState, bool in_bigEndian);

DLLEXPORT void utf32_StateReset(UTF32_State *out_pState);

DLLEXPORT ParseBlocker utf32_parse(
	void *in_out_pParserState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface);

#ifdef __cplusplus
}
#endif

#endif
