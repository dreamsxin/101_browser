#include "GIF/GIF.h"
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
	if (in_separator == 0x2C)
	{
		return read_TableBased_Image(in_gifFile);
	}

	// TODO
	return ReadResultNotImplemented;
}

ReadResult read_TableBased_Image(FILE* in_gifFile)
{
	ReadResult readResult;
	Image_Descriptor imageDescriptor;
	imageDescriptor.Image_Separator = 0x2C;

	readResult = read_Image_Descriptor(in_gifFile, &imageDescriptor);

	if (readResult != ReadResultOK)
		return readResult;

	if (imageDescriptor.Local_Color_Table_Flag)
	{
		return ReadResultNotImplemented;
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

ReadResult read_Image_Data(FILE* in_gifFile)
{
	uint8_t LZW_code_size;
	Data_SubBlock subBlock;

	if (fread(&LZW_code_size, sizeof(LZW_code_size), 1, in_gifFile) != 1)
		return ReadResultPrematureEndOfStream;

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

ReadResult read_Application_Extension(FILE* in_gifFile, bool in_is89a)
{
	if (!in_is89a)
		return ReadResultInvalidVersion;

	// because it is not yet implemented
	return ReadResultNotImplemented;
}

ReadResult read_Comment_Extension(FILE* in_gifFile, bool in_is89a)
{
	if (!in_is89a)
		return ReadResultInvalidVersion;

	// because it is not yet implemented
	return ReadResultNotImplemented;
}