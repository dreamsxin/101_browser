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

#ifndef _Unicode_Parser_UTF16_h
#define _Unicode_Parser_UTF16_h

#include "Unicode/Unicode.h"
#include "Unicode/Parser.h"
#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"
#include "IO/ByteStream_v4.h"

#ifdef __cplusplus
extern "C" {
#endif

// UTF-16 data structures
typedef enum
{
	UTF16_EntryPoint_BeforeReading,
	UTF16_EntryPoint_BeforeWritingReplacementCharacterAndGotoBegin, 
	UTF16_EntryPoint_WriteCodePoint, 
	UTF16_EntryPoint_WriteTwoTerminalReplacementCharacters, 
	UTF16_EntryPoint_WriteTerminalReplacementCharacter, 
	UTF16_EntryPoint_Terminated
} UTF16_EntryPoint;

typedef struct
{
	// Permanent settings
	bool bigEndian;

	// Temporary variables
	UTF16_EntryPoint entryPoint;
	uint8_t readCount;
	bool isSecondWord;
	/*
	* TODO: Optimize away frome these variables. But this is for the next
	* step.
	*/
	uint16_t prevWord;
	uint16_t currentWord;
	UnicodeCodePoint currentCodePoint;
} UTF16_State;

// UTF-16 functions
DLLEXPORT void utf16_StateInit(UTF16_State *out_pState, bool in_bigEndian);

DLLEXPORT void utf16_StateReset(UTF16_State *out_pState);

DLLEXPORT ParseBlocker utf16_parse(
	void *in_out_pParserState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface);

#ifdef __cplusplus
}
#endif

#endif
