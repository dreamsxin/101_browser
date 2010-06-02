#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool verifyCheckSum(FILE* fontFile, TableDirectory* in_pTableDirectory)
{
	bool headAdjustment = (in_pTableDirectory->tag.uint == CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));

	unsigned int size = ((in_pTableDirectory->length+3)& ~3)/4;
	fseek(fontFile, in_pTableDirectory->offset, SEEK_SET);
	unsigned int sum=0;

	for (size_t i=0; i<size; i++)
	{
		if (i==2 && headAdjustment)
		{
			if (fseek(fontFile, 4, SEEK_CUR) != 0)
			{
				return false;
			}
		}
		else
		{
			unsigned int currentDword;

			/*
			* This means the file was probably not large enough for the given size.
			* So we have an invalid checksum (this error is recoverable - that is why
			* we return false and don't call exit)
			*/
			if (fread(&currentDword, sizeof(currentDword), 1, fontFile) != 1)
			{
				return false;
			}

			sum+=convertEndianess(currentDword);
		}
	}

	return sum == in_pTableDirectory->checkSum;
}