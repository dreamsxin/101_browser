#include <cassert>
#include <climits>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_loca(FILE* fontFile, TrueTypeFont* in_trueTypeFont, 
					bool useLongTableVersion, Table_loca *in_lpTable_loca)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('l', 'o', 'c', 'a'));
	
	if (pTableRecord == NULL)
		return false;

	size_t numGlyfs;
	if (!useLongTableVersion)
	{
		/*
		 * The reason for the first condition holds should be obvious.
		 * The second condition comes from that according to
		 * http://www.microsoft.com/typography/otspec/loca.htm
		 * "By definition, index zero points to the “missing character,” which is 
		 * the character that appears if a character is not found in the font."
		 * This shows that the size has at least to be 2.
		 * Additionally we can read:
		 * " In the particular case of the last glyph(s), loca[n] will be equal 
		 * the length of the glyph data ('glyf') table."
		 * Because of the former condition the glyf table can't be empty. So there
		 * has to be a second entry different from the first and pTableRecord->length 
		 * has to be 4 or larger.
		 */
		if ((pTableRecord->length & 0x1) != 0 || pTableRecord->length <= 4)
			return false;
		else
			numGlyfs=pTableRecord->length/2-1;
	}
	else
	{
		/*
		 * The reason for the first condition holds should be obvious.
		 * The second condition comes from that according to
		 * http://www.microsoft.com/typography/otspec/loca.htm
		 * "By definition, index zero points to the “missing character,” which is 
		 * the character that appears if a character is not found in the font."
		 * This shows that the size has at least to be 4.
		 * Additionally we can read:
		 * " In the particular case of the last glyph(s), loca[n] will be equal 
		 * the length of the glyph data ('glyf') table."
		 * Because of the former condition the glyf table can't be empty. So there
		 * has to be a second entry different from the first and pTableRecord->length 
		 * has to be 8 or larger.
		 */
		if ((pTableRecord->length & 0x3) != 0 || pTableRecord->length <=8)
			return false;
		else
			numGlyfs=pTableRecord->length/4-1;
	}

	TableRecord* pTableRecord_glyf = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('g', 'l', 'y', 'f'));

	if (pTableRecord_glyf == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	/*
	 * This assertion is true because when we compute the
	 * value of numGlyfs we either divide an UINT (for which 
	 * sizeof(UINT) is less or equal sizeof(size_t)) value by
	 * 2 or 4 -- so it has to be < SIZE_MAX - 1 (even smaller --
	 * but we won't need more exactness ;-) )
	 */
	assert(numGlyfs < SIZE_MAX - 1);
	in_lpTable_loca->offsets.allocate(numGlyfs+1);

	UINT previousOffset = 0;

	for (size_t currentIndex = 0; currentIndex < in_lpTable_loca->offsets.count(); currentIndex++)
	{
		UINT readOffset;

		if (useLongTableVersion)
		{
			UINT readLongOffset;

			if (fread(&readLongOffset, sizeof(readLongOffset), 1, fontFile) != 1)
			{
				in_lpTable_loca->offsets.free();
				return false;
			}

			switchEndianess(&readLongOffset);

			readOffset = readLongOffset;
		}
		else
		{
			USHORT readShortOffset;

			if (fread(&readShortOffset, sizeof(readShortOffset), 1, fontFile) != 1)
			{
				in_lpTable_loca->offsets.free();
				return false;
			}

			switchEndianess(&readShortOffset);

			readOffset = 2*static_cast<UINT>(readShortOffset);
		}

		/*!
		 * According to
		 * http://www.microsoft.com/typography/otspec/loca.htm
		 * "The offsets must be in ascending order with loca[n] <= loca[n+1]."
		 */
		if (previousOffset > readOffset)
		{
			in_lpTable_loca->offsets.free();
			return false;
		}

		in_lpTable_loca->offsets.data()[currentIndex] = readOffset;

		previousOffset = readOffset;
	}

	// This invariant should follow from the code and is easy to prove
	assert(previousOffset == in_lpTable_loca->offsets.data()[in_lpTable_loca->offsets.count()-1]);

	/*!
	 * According to
	 * http://www.microsoft.com/typography/otspec/loca.htm
	 * "In the particular case of the last glyph(s), 
	 * loca[n] will be equal the length of the glyph data 
	 * ('glyf') table."
	 */
	if (previousOffset != pTableRecord_glyf->length)
	{
		in_lpTable_loca->offsets.free();
		return false;
	}

	for (size_t currentIndex = 0; currentIndex < in_lpTable_loca->offsets.count(); currentIndex++)
	{
		printf("%lu: %lu\n", currentIndex, in_lpTable_loca->offsets.data()[currentIndex]);
	}

	printf("\n");

	return true;
}
