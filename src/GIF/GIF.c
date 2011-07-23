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

#include "GIF/GIF.h"
#include "GIF/LZW.h"
#include "IO/BitRead.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

size_t bytesOfColorTable(unsigned char in_sizeOfColorTable)
{
	return 3*(1<<(in_sizeOfColorTable+1));
}

ReadResult read_GIF_Data_Stream(FILE* in_gifFile, GIF_Data_Stream *in_pDataStream)
{
	bool is89a;
	ReadResult readResult = read_Header(in_gifFile, &in_pDataStream->header, &is89a);
	
	if (readResult != ReadResultOK)
		return readResult;

	readResult = read_Logical_Screen(in_gifFile, &in_pDataStream->logicalScreen);

	if (readResult != ReadResultOK)
		return readResult;

	while (1)
	{
		uint8_t lIntroducer;

		if (fread(&lIntroducer, sizeof(lIntroducer), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		// Trailer
		if (0x3B == lIntroducer)
		{
			break;
		}

		readResult = read_Data(in_gifFile, lIntroducer, is89a);

		if (readResult != ReadResultOK)
			return readResult;
	}

	return ReadResultOK;
}

ReadResult read_Header(FILE* in_gifFile, Header *in_pHeader, bool *out_pIs89a)
{
	if (fread(in_pHeader, sizeof(*in_pHeader), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (strncmp(in_pHeader->Signature, "GIF", sizeof(in_pHeader->Signature)) != 0)
		return ReadResultInvalidData;

	if (strncmp(in_pHeader->Version, "87a", sizeof(in_pHeader->Version)) == 0)
	{
		*out_pIs89a = false;
		return ReadResultOK;
	}

	if (strncmp(in_pHeader->Version, "89a", sizeof(in_pHeader->Version)) == 0)
	{
		*out_pIs89a = true;
		return ReadResultOK;
	}
	
	return ReadResultInvalidData;
}

ReadResult read_SpecialPurpose_Block(FILE* in_gifFile, uint8_t in_label, bool in_is89a)
{
	switch (in_label)
	{
	case 0xFF:
		return read_Application_Extension(in_gifFile, in_is89a);
	case 0xFE:
		return read_Comment_Extension(in_gifFile, in_is89a);
	default:
		return ReadResultInvalidData;
	}
}

ReadResult read_Logical_Screen(FILE* in_gifFile, Logical_Screen *in_pLogicalScreen)
{
	if (fread(&in_pLogicalScreen->logicalScreenDescriptor, sizeof(Logical_Screen_Descriptor), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (in_pLogicalScreen->logicalScreenDescriptor.Global_Color_Table_Flag)
	{
		size_t bytesOfGlobalColorTable = bytesOfColorTable(in_pLogicalScreen->logicalScreenDescriptor.Size_Of_Global_Color_Table);

		in_pLogicalScreen->globalColorTable = (uint8_t*) malloc(bytesOfGlobalColorTable);

		if (in_pLogicalScreen->globalColorTable == NULL)
			return ReadResultAllocationFailure;

		if (fread(in_pLogicalScreen->globalColorTable, bytesOfGlobalColorTable, 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;
	}
	else
	{
		in_pLogicalScreen->globalColorTable = NULL;
	}

	return ReadResultOK;
}

ReadResult read_Data(FILE* in_gifFile, uint8_t in_introducer, bool in_is89a)
{
	uint8_t lLabel;

	if (0x21 == in_introducer)
	{
		if (fread(&lLabel, sizeof(lLabel), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		/*
		* We have the following situation:
		*
		* ┌─────────────────────┬─────────┬───────────────────────┐
		* │Extension Introducer │ GIF 87a │ GIF 89a               │
		* ├─────────────────────┼─────────┼───────────────────────┤
		* │0xFE or 0xFF         │         │ Special Purpose Block │
		* │0xF9 or 0xF1         │  skip   │     Graphic Block     │
		* │other                │         │         skip          │
		* └─────────────────────┴─────────┴───────────────────────┘
		*/
		if (in_is89a)
		{
			/*
			* 0xFF: Application Extension
			* 0xFE: Comment Extension
			*/
			if (0xFF == lLabel || 0xFE == lLabel)
			{
				return read_SpecialPurpose_Block(in_gifFile, lLabel, in_is89a);
			}
			
			/*
			* 0xF9: Graphic Control Extension
			* 0x01: Plain Text Extension
			*/
			else if (0xF9 == lLabel || 0x01 == lLabel)
			{
				return read_Graphic_Block(in_gifFile, in_introducer, lLabel, in_is89a);
			}
			else
				return skipBlock(in_gifFile);
		}
		
	}
	else if (0x2C == in_introducer)
	{
		return read_Graphic_Block(in_gifFile, in_introducer, 0, in_is89a);
	}
	else
		return ReadResultInvalidData;
}

ReadResult read_Graphic_Block(FILE* in_gifFile, uint8_t in_separator, uint8_t in_label, bool in_is89a)
{
	if (0x21 == in_separator && 0xF9 == in_label)
	{
		ReadResult lReadResult = read_Graphic_Control_Extension(in_gifFile, in_is89a);

		if (lReadResult != ReadResultOK)
			return lReadResult;

		if (fread(&in_separator, sizeof(in_separator), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		if (0x2C == in_separator)
			goto return_read_GraphicRendering_Block;
		else if (0x21 == in_separator)
		{
			if (fread(&in_label, sizeof(in_label), 1, in_gifFile) != 1)
				return ReadResultPrematureEndOfStream;

			/*
			* TODO skip block (except Plain Text Extension) - it 
			* does not belong here
			*/
			return ReadResultNotImplemented;
		}
		else
			return ReadResultInvalidData;
	}
	else if (0x2C == in_separator)
	{
return_read_GraphicRendering_Block:
		return read_GraphicRendering_Block(in_gifFile, in_separator);
	}
	
	// TODO
	return ReadResultNotImplemented;
}

ReadResult read_GraphicRendering_Block(FILE* in_gifFile, uint8_t in_separator)
{
	TableBased_Image tableBasedImage;

	if (in_separator == 0x2C)
	{
		return read_TableBased_Image(in_gifFile, &tableBasedImage);
	}

	// TODO
	return ReadResultNotImplemented;
}

ReadResult read_TableBased_Image(FILE* in_gifFile, TableBased_Image *in_pTableBasedImage)
{
	ReadResult readResult;
	in_pTableBasedImage->imageDescriptor.Image_Separator = 0x2C;

	readResult = read_Image_Descriptor(in_gifFile, &in_pTableBasedImage->imageDescriptor);

	if (readResult != ReadResultOK)
		return readResult;

	if (in_pTableBasedImage->imageDescriptor.Local_Color_Table_Flag)
	{
		size_t bytesOfGlobalColorTable = bytesOfColorTable(in_pTableBasedImage->imageDescriptor.Size_of_Local_Color_Table);

		in_pTableBasedImage->localColorTable = (uint8_t*) malloc(bytesOfGlobalColorTable);

		if (in_pTableBasedImage->localColorTable == NULL)
			return ReadResultAllocationFailure;

		if (fread(in_pTableBasedImage->localColorTable, bytesOfGlobalColorTable, 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;
	}
	else
	{
		in_pTableBasedImage->localColorTable = NULL;
	}

	return read_Image_Data(in_gifFile);
}

ReadResult read_Graphic_Control_Extension(FILE* in_gifFile, bool in_is89a)
{
	Graphic_Control_Extension graphicControlExtension;
	uint8_t terminator;
	
	if (!in_is89a)
	{
		return skipBlock(in_gifFile);
	}

	if (fread(&graphicControlExtension, sizeof(graphicControlExtension), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (graphicControlExtension.Block_Size != 4)
		return ReadResultInvalidData;

	if (graphicControlExtension.Reserved != 0)
		return ReadResultInvalidData;

	if (fread(&terminator, sizeof(terminator), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (terminator != 0x00)
	{
		return ReadResultInvalidData;
	}

	return ReadResultOK;
}

ReadResult read_Image_Descriptor(FILE* in_gifFile, Image_Descriptor* in_pImageDescriptor)
{
	// sizeof(*in_pImageDescriptor)-1 since we have already read the first byte
	if (fread(&in_pImageDescriptor->Image_Left_Position, sizeof(*in_pImageDescriptor)-1, 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (in_pImageDescriptor->Reserved != 0)
		return ReadResultInvalidData;

	return ReadResultOK;
}

typedef struct
{
	FILE* file;
	uint8_t Block_Size_Bytes;
	uint8_t Read_Bytes;
	bool endOfStream;
} Image_Data_StreamState;

void init_Image_Data_StreamState(Image_Data_StreamState *in_pStreamState, FILE* in_file)
{
	in_pStreamState->file = in_file;
	in_pStreamState->Block_Size_Bytes = 0;
	in_pStreamState->Read_Bytes = 0;
	in_pStreamState->endOfStream = false;
}

//bool read_Image_Data_Byte(void *in_pStreamState, uint8_t* in_pBuffer)
size_t read_Image_Data_byteStreamInterface(void *in_out_pByteStreamState, void *out_pBuffer, size_t in_count)
{
	Image_Data_StreamState *pStreamState = (Image_Data_StreamState*) in_out_pByteStreamState;

	// A limitation of this function is that we can read only one byte at a time
	assert(in_count == 1);

	if (in_count != 1)
		return 0;

	if (pStreamState->endOfStream)
		return 0;

	/*
	* If we read the current block completely, we read the size of the next block
	*/
	if (pStreamState->Read_Bytes == pStreamState->Block_Size_Bytes)
	{
		if (fread(&pStreamState->Block_Size_Bytes, sizeof(pStreamState->Block_Size_Bytes), 1, pStreamState->file) != 1)
			return 0;

		if (pStreamState->Block_Size_Bytes == 0)
		{
			pStreamState->endOfStream = true;
			return 0;
		}

		pStreamState->Read_Bytes = 0;
	}

	if (fread(out_pBuffer, 1, 1, pStreamState->file) != 1)
	{
		pStreamState->endOfStream = true;
		return 0;
	}

	pStreamState->Read_Bytes++;

	return 1;
}

ReadResult read_Image_Data(FILE* in_gifFile)
{
	uint8_t LZW_Minimum_Code_Size;
	BitReadState bitReadState;
	ByteStreamInterface bitReadInterface;
	Image_Data_StreamState streamState;
	LZW_Tree *pTree;
	LZW_Stack *pStack;

	uint16_t startCode;
	uint16_t stopCode;
	uint16_t currentTableIndex;
	uint8_t currentCodeWordBitCount;

	uint16_t currentCodeWord;

	// Initialize bitReadInterface
	memset(&bitReadInterface, 0, sizeof(bitReadInterface));
	bitReadInterface.mpfRead = read_Image_Data_byteStreamInterface;

#if 0
	size_t pixelsWritten = 0;
#endif

	if (fread(&LZW_Minimum_Code_Size, sizeof(LZW_Minimum_Code_Size), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (LZW_Minimum_Code_Size < 2 || LZW_Minimum_Code_Size > 8)
		return ReadResultInvalidData;
	
	startCode = 1<<LZW_Minimum_Code_Size;
	// ASGN:GIF_314
	stopCode = startCode+1;

	initBitReadState(&bitReadState, &streamState, bitReadInterface);
	init_Image_Data_StreamState(&streamState, in_gifFile);

	pTree = (LZW_Tree *) malloc(sizeof(LZW_Tree));

	if (pTree == NULL)
	{
		return ReadResultAllocationFailure;
	}

	pStack = (LZW_Stack *) malloc(sizeof(LZW_Stack));

	if (pStack == NULL)
	{
		free(pTree);
		return ReadResultAllocationFailure;
	}

	initLZW_Tree(pTree, 1<<LZW_Minimum_Code_Size);
	currentTableIndex = stopCode + 1;
	currentCodeWordBitCount = LZW_Minimum_Code_Size+1;
	
	while (1)
	{
		/*
		 * Q: Why is it necessary to set currentCodeWord to 0?
		 * A: Since the code word can have 8 or less bits, the higher byte would
		 *    not be initialized correctly otherwise.
		 *
		 *    To understand this, imagine that we wouldn't set it to 0 (i. e. the
		 *    upper byte has some arbitrary value) and read 8 or less bits.
		 *    readBitsLittleEndian will set the correct value for the lower byte (since this
		 *    is what it is supposed to do). But it will not change the value
		 *    of the upper byte, meaning we get a wrong code word.
		 */
		currentCodeWord = 0;

		if (readBitsLittleEndian(&bitReadState, &currentCodeWord, currentCodeWordBitCount) != currentCodeWordBitCount)
		{
			free(pTree);
			free(pStack);
			return ReadResultPrematureEndOfStream;
		}

		if (currentCodeWord >= currentTableIndex)
		{
			free(pTree);
			free(pStack);
			return ReadResultInvalidData;
		}

		/*! 
		 * Q: Why do we first check whether
		 * startCode == currentCodeWord or
		 * stopCode == currentCodeWord
		 * and then check if
		 * currentTableIndex > 4096 ?
		 * 
		 * A: Because 
		 * startCode == currentCodeWord or
		 * stopCode == currentCodeWord
		 * can also occur at table index 4097. For example the file
		 * "testfiles/gif/wikipedia/Rotating_earth_(small).gif"
		 * provides such a test case.
		 */
		// CND:GIF_375
		if (startCode == currentCodeWord)
		{
			/*
			 * Reiniting the tree is not (!) necessary at the moment - 
			 * but if we change the implementation it could become...
			 */

			currentTableIndex = stopCode + 1;
			currentCodeWordBitCount = LZW_Minimum_Code_Size+1;

			continue;
		}
		// CND:GIF_388
		else if (stopCode == currentCodeWord)
		{
			break;
		}
		/*
		 * Table indices > 4096 are not allowed. So if this occures,
		 * we have an invalid stream.
		 */
		// CND:GIF_395
		else if (currentTableIndex > 4096)
		{
			free(pTree);
			free(pStack);
			return ReadResultInvalidData;
		}
		else
		{
			LZW_Tree_Node *pCurrentNode;

			// Follows from CND:GIF_395
			assert(currentTableIndex <= 4096);

#if 0
			printf("%u %u\n", currentTableIndex, currentCodeWord);
#endif
			// CND:GIF_415
			if (currentCodeWord < startCode)
			{
				pTree->nodes[currentTableIndex].pPrev = NULL;
				pTree->nodes[currentTableIndex].firstCode = (uint8_t) currentCodeWord;
				pTree->nodes[currentTableIndex].lastCode = (uint8_t) currentCodeWord;
			}
			else
			{
				/*
				 * From CND:GIF_375 and CND:GIF_388 it follows that
				 * currentCodeWord != startCode
				 * and
				 * currentCodeWord != stopCode
				 * From ASGN:GIF_314 and because there is no subsequent assignment to
				 * either startCode, stopCode or currentCodeWord we get:
				 * currentCodeWord > stopCode or currentCodeWord < startCode
				 * But the second case can't occur because of CND:GIF_415.
				 */
				assert(currentCodeWord > stopCode);

				pCurrentNode = pTree->nodes+currentCodeWord;

				pTree->nodes[currentTableIndex].pPrev = pCurrentNode;
				pTree->nodes[currentTableIndex].firstCode = pCurrentNode->firstCode;
				pTree->nodes[currentTableIndex].lastCode = (pCurrentNode+1)->firstCode;
			}

			initLZW_Stack(pStack);

			pCurrentNode = pTree->nodes+currentTableIndex;

			while (pCurrentNode != NULL)
			{
				pStack->pNodes[pStack->stackSize] = pCurrentNode;
				pStack->stackSize++;
				pCurrentNode = pCurrentNode->pPrev;
			}

			while (pStack->stackSize != 0)
			{
				pCurrentNode = pStack->pNodes[pStack->stackSize-1];
				pStack->stackSize--;

#if 0
				printf("%u ", pCurrentNode->lastCode);
#endif

#if 0
				pixelsWritten++;
#endif
			}
		}

		switch (currentTableIndex)
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
			currentCodeWordBitCount++;
			break;
		}

		currentTableIndex++;
	}

	if (streamState.Read_Bytes != streamState.Block_Size_Bytes)
	{
		return ReadResultInvalidData;
	}

	if (fread(&streamState.Block_Size_Bytes, sizeof(streamState.Block_Size_Bytes), 1, in_gifFile) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}

	// If there is no terminator block return failure
	if (0 != streamState.Block_Size_Bytes)
	{
		return ReadResultInvalidData;
	}

#if 0
	printf("Pixels written: %u\n", pixelsWritten);
#endif

	free(pStack);
	free(pTree);

	return ReadResultOK;
}

ReadResult read_Application_Extension(FILE* in_gifFile, bool in_is89a)
{
	Application_Extension applExt;
	
	if (!in_is89a)
		return ReadResultInvalidVersion;

	if (fread(&applExt, sizeof(applExt), 1, in_gifFile) != 1)
	{
		return ReadResultPrematureEndOfStream;
	}

	if (applExt.Block_Size != 11)
	{
		return ReadResultInvalidData;
	}

	if (strncmp(applExt.Application_Identifier, "NETSCAPE", 8) == 0 && 
		strncmp(applExt.Application_Authentication_Code, "2.0", 3) == 0)
	{
		uint8_t blockSize;
		uint8_t blockData[3];

		if (fread(&blockSize, sizeof(blockSize), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		if (blockSize != 3)
			return ReadResultInvalidData;

		if (fread(blockData, sizeof(blockData), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		// TODO: Interprete read data

		if (fread(&blockSize, sizeof(blockSize), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		if (blockSize != 0)
			return ReadResultInvalidData;
	}
	else
	{
		return ReadResultNotImplemented;
	}

	return ReadResultOK;
}

ReadResult read_Comment_Extension(FILE* in_gifFile, bool in_is89a)
{
	Data_SubBlock subBlock;

	if (!in_is89a)
		return ReadResultInvalidVersion;

	if (fread(&subBlock.Block_Size, sizeof(subBlock.Block_Size), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	while (subBlock.Block_Size != 0)
	{
		subBlock.Data_Values = (uint8_t*) malloc(subBlock.Block_Size);

		if (NULL == subBlock.Data_Values)
		{
			return ReadResultAllocationFailure;
		}

		if (fread(subBlock.Data_Values, subBlock.Block_Size, 1, in_gifFile) != 1)
		{
			return ReadResultPrematureEndOfStream;
		}

		// TODO: Interprete read block

		free(subBlock.Data_Values);

		if (fread(&subBlock.Block_Size, sizeof(subBlock.Block_Size), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;
	}

	return ReadResultOK;
}

ReadResult skipBlock(FILE* in_gifFile)
{
	uint8_t Block_Size;
	
	if (fread(&Block_Size, sizeof(Block_Size), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	while (Block_Size != 0)
	{
		size_t idx;
		uint8_t buffer;

		for (idx = 0; idx < Block_Size; idx++)
		{
			if (fread(&buffer, 1, 1, in_gifFile) != 1)
			{
				return ReadResultPrematureEndOfStream;
			}
		}

		if (fread(&Block_Size, sizeof(Block_Size), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;
	}
	
	return ReadResultOK;
}
