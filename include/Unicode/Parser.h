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
#include "IO/ByteStream_v4.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	/*
	* ParseBlocker_Neither is returned when parsing is terminated successfully 
	* and there is no need to proceed further.
	*/
	ParseBlocker_Neither,
	/*
	* ParseBlocker_Reader and ParseBlocker_Writer are returned when there 
	* is a problem (either intervention required or error) in the reader 
	* or writer, respectively.
	*/
	ParseBlocker_Reader,
	ParseBlocker_Writer
} ParseBlocker;


// UTF-8 data structures
typedef enum
{
	UTF8_InternalState_Start,

	UTF8_InternalState_X_error,
	UTF8_InternalState_X_X_error,
	UTF8_InternalState_X_X_X_error,
	UTF8_InternalState_X_X_X_X_error,
	UTF8_InternalState_X_X_X_X_X_error,

	UTF8_InternalState_X_append_emit,
	UTF8_InternalState_X_X_append_emit,
	UTF8_InternalState_X_X_X_append_emit,

	UTF8_InternalState_E0,
	UTF8_InternalState_ED,
	UTF8_InternalState_F0,
	UTF8_InternalState_F4
} UTF8_InternalState;

typedef enum
{
	UTF8_EntryPoint_Begin,
	UTF8_EntryPoint_WriteTerminalReplacementCharacter,
	UTF8_EntryPoint_Start_leq_0x7F,
	UTF8_EntryPoint_Start_between_0x80_0xBF,
	UTF8_EntryPoint_Start_FE_FF,
	UTF8_EntryPoint_not_Start_leq_0x7F_or_geq_0xC0,
	UTF8_EntryPoint_X_error_between_0x80_0xBF,
	UTF8_EntryPoint_X_append_emit_between_0x80_0xBF,
	UTF8_EntryPoint_Terminated
} UTF8_EntryPoint;

typedef struct
{
	UTF8_EntryPoint entryPoint;
	UTF8_InternalState internalState;
	UnicodeCodePoint codePoint;
	uint8_t currentByte;
} UTF8_State;


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


// UTF-8 functions
DLLEXPORT void utf8_StateInit(UTF8_State *out_pState);

DLLEXPORT void utf8_StateReset(UTF8_State *out_pState);

DLLEXPORT ParseBlocker utf8_parse(
	void *in_out_pByteStreamState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface);


// UTF-16 functions
DLLEXPORT void utf16_StateInit(UTF16_State *out_pState, bool in_bigEndian);

DLLEXPORT void utf16_StateReset(UTF16_State *out_pState);

DLLEXPORT ParseBlocker utf16_parse(
	void *in_out_pByteStreamState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface);


// UTF-32 functions
DLLEXPORT void utf32_StateInit(UTF32_State *out_pState, bool in_bigEndian);

DLLEXPORT void utf32_StateReset(UTF32_State *out_pState);

DLLEXPORT ParseBlocker utf32_parse(
	void *in_out_pByteStreamState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface);

#ifdef __cplusplus
}
#endif

#endif
