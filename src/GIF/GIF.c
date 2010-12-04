#include "GIF/GIF.h"
#include "GIF/LZW.h"
#include "Algorithm/BitRead.h"
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

	if (in_introducer == 0x21)
	{
		if (fread(&lLabel, sizeof(lLabel), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		if (0xFF == lLabel || 0xFE == lLabel)
		{
			return read_SpecialPurpose_Block(in_gifFile, lLabel, in_is89a);
		}
		else if (0xF9 == lLabel)
		{
			ReadResult lReadResult = read_Graphic_Control_Extension(in_gifFile, in_is89a);

			if (lReadResult != ReadResultOK)
				return lReadResult;

			{
				uint8_t lSeparator;

				if (fread(&lSeparator, sizeof(lSeparator), 1, in_gifFile) != 1)
					return ReadResultPrematureEndOfStream;

				return read_Graphic_Block(in_gifFile, lSeparator);
			}
		}
	}
	else if (in_introducer == 0x2C)
	{
		return read_Graphic_Block(in_gifFile, in_introducer);
	}

	return ReadResultInvalidData;
}

ReadResult read_Graphic_Block(FILE* in_gifFile, uint8_t in_separator)
{
	if (in_separator == 0x2C)
	{
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
		return ReadResultInvalidVersion;
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

bool read_Image_Data_Byte(void *in_pStreamState, uint8_t* in_pBuffer)
{
	Image_Data_StreamState *pStreamState = (Image_Data_StreamState*) in_pStreamState;

	if (pStreamState->endOfStream)
		return false;

	if (pStreamState->Read_Bytes == pStreamState->Block_Size_Bytes)
	{
		if (fread(&pStreamState->Block_Size_Bytes, sizeof(pStreamState->Block_Size_Bytes), 1, pStreamState->file) != 1)
			return false;

		if (pStreamState->Block_Size_Bytes == 0)
		{
			pStreamState->endOfStream = true;
			return false;
		}

		pStreamState->Read_Bytes = 0;
	}

	if (fread(in_pBuffer, 1, 1, pStreamState->file) != 1)
	{
		pStreamState->endOfStream = true;
		return false;
	}

	pStreamState->Read_Bytes++;

	return true;
}

#define PRINT_IMAGE 1

ReadResult read_Image_Data(FILE* in_gifFile)
{
	uint8_t LZW_Minimum_Code_Size;
	BitReadState bitReadState;
	Image_Data_StreamState streamState;
	LZW_Tree *pTree;
	LZW_Stack *pStack;

	uint16_t startCode;
	uint16_t stopCode;
	uint16_t currentTableIndex;
	uint8_t currentCodeWordBitCount;

	uint16_t currentCodeWord;

	size_t pixelsWritten = 0;

	if (fread(&LZW_Minimum_Code_Size, sizeof(LZW_Minimum_Code_Size), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

	if (LZW_Minimum_Code_Size < 2 || LZW_Minimum_Code_Size > 8)
		return ReadResultInvalidData;
	
	startCode = 1<<LZW_Minimum_Code_Size;
	stopCode = startCode+1;

	initBitReadState(&bitReadState);
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
		 * A: Since the code word can have 8 or less bits, the higher nibble would
		 *    not be initialized correctly.
		 */
		currentCodeWord = 0;

		if (!readBits(&bitReadState, &streamState, read_Image_Data_Byte, &currentCodeWord, currentCodeWordBitCount))
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

		if (currentCodeWord == startCode)
		{
			/*
			 * Reiniting the tree is not (!) necessary - but if we change the implementation
			 * it could become...
			 */

			currentTableIndex = stopCode + 1;
			currentCodeWordBitCount = LZW_Minimum_Code_Size+1;

			continue;
		}
		/*
		 * Table indices > 4096 are not allowed. So if this occures,
		 * we have an invalid stream.
		 */
		else if (currentTableIndex > 4096 && currentCodeWord != startCode)
		{
			free(pTree);
			free(pStack);
			return ReadResultInvalidData;
		}
		else if (currentCodeWord == stopCode)
		{
			break;
		}
		else
		{
			LZW_Tree_Node *pCurrentNode;

			//printf("%u %u\n", currentTableIndex, currentCodeWord);

			if (currentCodeWord < startCode)
			{
				pTree->nodes[currentTableIndex].firstCode = (uint8_t) currentCodeWord;
				pTree->nodes[currentTableIndex].lastCode = (uint8_t) currentCodeWord;
				pTree->nodes[currentTableIndex].pPrev = NULL;
			}
			else
			{
				assert(currentCodeWord > stopCode);

				pTree->nodes[currentTableIndex].pPrev = pTree->nodes+currentCodeWord;
				pTree->nodes[currentTableIndex].firstCode = pTree->nodes[currentTableIndex].pPrev->firstCode;
				pTree->nodes[currentTableIndex].lastCode = pTree->nodes[currentCodeWord+1].firstCode;
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

#if PRINT_IMAGE == 0
				printf("%u ", pCurrentNode->lastCode);
#endif

#if PRINT_IMAGE == 1
				if (pCurrentNode->firstCode == 40)
				{
					printf("Û");
				}
				else if (pCurrentNode->firstCode == 255)
				{
					printf(" ");
				}
#endif

				pixelsWritten++;

#if PRINT_IMAGE == 1
				if (pixelsWritten % 32 == 0)
					printf("\n");
#endif
			}

#if PRINT_IMAGE == 0
			printf("\n");
#endif
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

	// If there is no terminator block return failure
	if (readBits(&bitReadState, &streamState, read_Image_Data_Byte, &currentCodeWord, currentCodeWordBitCount))
	{
		return ReadResultInvalidData;
	}

	printf("Pixels written: %u\n", pixelsWritten);

	free(pStack);
	free(pTree);

	return ReadResultOK;
}

ReadResult read_Application_Extension(FILE* in_gifFile, bool in_is89a)
{
	Application_Extension applExt;
	Data_SubBlock subBlock;
	
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
		// TODO
	}
	else
	{
		return ReadResultNotImplemented;
	}

	while (1)
	{
		if (fread(&subBlock.Block_Size, sizeof(subBlock.Block_Size), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		if (subBlock.Block_Size == 0)
			break;

		subBlock.Data_Values = (uint8_t*) malloc(subBlock.Block_Size);

		if (fread(subBlock.Data_Values, subBlock.Block_Size, 1, in_gifFile) != 1)
		{
			return ReadResultPrematureEndOfStream;
		}

		// TODO: Interprete read block

		free(subBlock.Data_Values);
	}

	return ReadResultOK;
}

ReadResult read_Comment_Extension(FILE* in_gifFile, bool in_is89a)
{
	Data_SubBlock subBlock;

	if (!in_is89a)
		return ReadResultInvalidVersion;

	while (1)
	{
		if (fread(&subBlock.Block_Size, sizeof(subBlock.Block_Size), 1, in_gifFile) != 1)
			return ReadResultPrematureEndOfStream;

		if (subBlock.Block_Size == 0)
			break;

		subBlock.Data_Values = (uint8_t*) malloc(subBlock.Block_Size);

		if (fread(subBlock.Data_Values, subBlock.Block_Size, 1, in_gifFile) != 1)
		{
			return ReadResultPrematureEndOfStream;
		}

		// TODO: Interprete read block

		free(subBlock.Data_Values);
	}

	return ReadResultOK;
}
