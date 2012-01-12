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

#ifndef _LZW_h
#define _LZW_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint16_t previousLzwTreeNodeIndex;
	uint8_t firstCode;
	uint8_t lastCode;
} LZW_TreeNode;

/*!
 * You should not allocate this on a stack. Use the heap or
 * a global variable instead.
 */
typedef struct
{
	 LZW_TreeNode treeNodes[4097];
	
	struct
	{
		uint16_t stackSize;
		uint16_t lzwTreeIndices[4097];
	} stack;

	uint8_t LZW_Minimum_Code_Size;
	uint16_t startCode;
	uint16_t stopCode;
	uint16_t currentTableIndex;
	uint8_t currentCodeWordBitCount;
} LZW_Decoder;

void initLZW_Decoder(LZW_Decoder *out_pLZW_Decoder, uint8_t in_LZW_Minimum_Code_Size);
uint8_t LZW_Decoder_getCurrentCodeWordBitCount(const LZW_Decoder *in_pLZW_Decoder);

typedef enum
{
	// When start code is handled - you do nothing
	LZW_DecoderAction_DoNothing,
	// When stop code is handled - you stop
	LZW_DecoderAction_Terminate,
	// You read data
	LZW_DecoderAction_DataAvailable,

	// On all these error conditions you better terminate
	LZW_DecoderAction_Error_CodewordGeqCurrentTableIndex,
	LZW_DecoderAction_Error_CodewordGeqColorTableSize
} LZW_DecoderAction;

LZW_DecoderAction LZW_Decoder_handleCodeword(LZW_Decoder *in_out_pLZW_Decoder, 
	uint16_t in_currentCodeword, uint16_t in_colorTableSize);

#ifdef __cplusplus
}
#endif

#endif
