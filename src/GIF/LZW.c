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

#include "GIF/LZW.h"
#include <assert.h>
#include <stdlib.h>
#include <assert.h>


/*
* Q: What do we need this for?
* A: Because some LZW Tree Nodes have no predecessor and we store indices 
*    instead of pointers, we need some value that fills the role of the NULL
*    pointer. This is what this sentinel is for.
*/
#define PREVIOUS_LZW_TREE_NODE_INDEX_SENTINEL UINT16_MAX

// a kind of private function
void LZW_Decoder_resetStack(LZW_Decoder *out_pLZW_Decoder)
{
	out_pLZW_Decoder->stack.stackSize = 0;
}

void initLZW_Decoder(LZW_Decoder *out_pLZW_Decoder, uint8_t in_LZW_Minimum_Code_Size)
{
	// Init tree nodes
	uint16_t i;
	
	out_pLZW_Decoder->LZW_Minimum_Code_Size = in_LZW_Minimum_Code_Size;
	out_pLZW_Decoder->startCode = 1 << in_LZW_Minimum_Code_Size;
	out_pLZW_Decoder->stopCode = out_pLZW_Decoder->startCode+1;
	out_pLZW_Decoder->currentTableIndex = out_pLZW_Decoder->stopCode + 1;
	out_pLZW_Decoder->currentCodeWordBitCount = in_LZW_Minimum_Code_Size+1;

	for (i = 0; i < out_pLZW_Decoder->startCode; i++)
	{
		out_pLZW_Decoder->treeNodes[i].previousLzwTreeNodeIndex = PREVIOUS_LZW_TREE_NODE_INDEX_SENTINEL;
		out_pLZW_Decoder->treeNodes[i].firstCode = (uint8_t) i;
		out_pLZW_Decoder->treeNodes[i].lastCode = (uint8_t) i;
	}

	// Init stack
	LZW_Decoder_resetStack(out_pLZW_Decoder);
}

uint8_t LZW_Decoder_getCurrentCodeWordBitCount(const LZW_Decoder *in_pLZW_Decoder)
{
	return in_pLZW_Decoder->currentCodeWordBitCount;
}

LZW_DecoderAction LZW_Decoder_handleCodeword(LZW_Decoder *in_out_pLZW_Decoder, 
	uint16_t in_currentCodeword, uint16_t in_colorTableSize)
{
	if (in_currentCodeword >= in_out_pLZW_Decoder->currentTableIndex)
		return LZW_DecoderAction_Error_CodewordGeqCurrentTableIndex;
	else if (in_currentCodeword == in_out_pLZW_Decoder->startCode)
	{
		in_out_pLZW_Decoder->currentTableIndex = in_out_pLZW_Decoder->stopCode + 1;
		in_out_pLZW_Decoder->currentCodeWordBitCount = in_out_pLZW_Decoder->LZW_Minimum_Code_Size+1;
		return LZW_DecoderAction_DoNothing;
	}
	else if (in_currentCodeword == in_out_pLZW_Decoder->stopCode)
	{
		return LZW_DecoderAction_Terminate;
	}
	else
	{
		LZW_TreeNode *pCurrentNode;

		assert(in_out_pLZW_Decoder->currentTableIndex <= 4096);

		// CND:LZW_85
		if (in_currentCodeword < in_out_pLZW_Decoder->startCode)
		{
			// CND:LZW_90
			if (in_currentCodeword >= in_colorTableSize)
			{
				return LZW_DecoderAction_Error_CodewordGeqColorTableSize;
			}
			
			in_out_pLZW_Decoder->treeNodes[in_out_pLZW_Decoder->currentTableIndex].previousLzwTreeNodeIndex = PREVIOUS_LZW_TREE_NODE_INDEX_SENTINEL;
			in_out_pLZW_Decoder->treeNodes[in_out_pLZW_Decoder->currentTableIndex].firstCode = (uint8_t) in_currentCodeword;
			in_out_pLZW_Decoder->treeNodes[in_out_pLZW_Decoder->currentTableIndex].lastCode = (uint8_t) in_currentCodeword;
		}
		else
		{
			/*
			 * From above it follows that
			 * in_currentCodeword != startCode
			 * and
			 * in_currentCodeword != stopCode
			 * From ASGN:GIF_314 and because there is no subsequent assignment to
			 * either startCode, stopCode or currentCodeWord we get:
			 * currentCodeWord > stopCode or currentCodeWord < startCode
			 * But the second case can't occur because of CND:LZW_85.
			 */
			assert(in_currentCodeword > in_out_pLZW_Decoder->stopCode);

			pCurrentNode = in_out_pLZW_Decoder->treeNodes+in_currentCodeword;

			in_out_pLZW_Decoder->treeNodes[in_out_pLZW_Decoder->currentTableIndex].previousLzwTreeNodeIndex = 
				in_currentCodeword;
			in_out_pLZW_Decoder->treeNodes[in_out_pLZW_Decoder->currentTableIndex].firstCode = 
				pCurrentNode->firstCode;
			in_out_pLZW_Decoder->treeNodes[in_out_pLZW_Decoder->currentTableIndex].lastCode = 
				(pCurrentNode+1)->firstCode;
		}

		LZW_Decoder_resetStack(in_out_pLZW_Decoder);

		pCurrentNode = in_out_pLZW_Decoder->treeNodes+in_out_pLZW_Decoder->currentTableIndex;
		
		assert(pCurrentNode != NULL);
		in_out_pLZW_Decoder->stack.lzwTreeIndices[in_out_pLZW_Decoder->stack.stackSize] = 
			in_out_pLZW_Decoder->currentTableIndex;
		in_out_pLZW_Decoder->stack.stackSize++;

		while (pCurrentNode->previousLzwTreeNodeIndex != PREVIOUS_LZW_TREE_NODE_INDEX_SENTINEL)
		{
			pCurrentNode = in_out_pLZW_Decoder->treeNodes+pCurrentNode->previousLzwTreeNodeIndex;
			in_out_pLZW_Decoder->stack.lzwTreeIndices[in_out_pLZW_Decoder->stack.stackSize] = 
				in_out_pLZW_Decoder->currentTableIndex;
			in_out_pLZW_Decoder->stack.stackSize++;
		}

		/*
		* Now we could be able to use the stack to write the decompressed data.
		* We do post processing for the function to work properly when called 
		* again.
		*/

		switch (in_out_pLZW_Decoder->currentTableIndex)
		{
		case 1<<3:
		case 1<<4:
		case 1<<5:
		case 1<<6:
		case 1<<7:
		case 1<<8:
		case 1<<9:
		case 1<<10:
		case 1<<11:
			in_out_pLZW_Decoder->currentCodeWordBitCount++;
			break;
		}

		/*
		* To quote from GIF 89a specification:
		* "There has been confusion about where clear codes can be found in the
		* data stream.  As the specification says, they may appear at anytime.  There
		* is not a requirement to send a clear code when the string table is full.
		* 
		* It is the encoder's decision as to when the table should be cleared.  When
		* the table is full, the encoder can chose to use the table as is, making no
		* changes to it until the encoder chooses to clear it.  The encoder during
		* this time sends out codes that are of the maximum Code Size.
		* 
		* As we can see from the above, when the decoder's table is full, it must
		* not change the table until a clear code is received.  The Code Size is that
		* of the maximum Code Size.  Processing other than this is done normally."
		*/
		assert(in_out_pLZW_Decoder->currentTableIndex <= 4096);

		if (in_out_pLZW_Decoder->currentTableIndex < 4096)
			in_out_pLZW_Decoder->currentTableIndex++;

		assert(in_out_pLZW_Decoder->currentTableIndex <= 4096);

		return LZW_DecoderAction_DataAvailable;
	}	
}

int LZW_Decoder_popPaletteIndex(LZW_Decoder *in_out_pLZW_Decoder, 
	uint8_t *out_pCurrentPaletteIndex)
{
	if (in_out_pLZW_Decoder->stack.stackSize != 0)
	{
		*out_pCurrentPaletteIndex = in_out_pLZW_Decoder->treeNodes[ 
					in_out_pLZW_Decoder->stack.lzwTreeIndices[in_out_pLZW_Decoder->stack.stackSize-1]].lastCode;

		in_out_pLZW_Decoder->stack.stackSize--;

		return 0;
	}
	else
		return 1;
}
