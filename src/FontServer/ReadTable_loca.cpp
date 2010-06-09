#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_loca(FILE* fontFile, TrueTypeFont* in_trueTypeFont, 
					USHORT in_numGlyphs, bool useLongTableVersion,
					Table_loca *in_lpTable_loca)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('l', 'o', 'c', 'a'));
	
	if (pTableRecord == NULL)
		return false;

	/*!
	 * According to 
	 * http://www.microsoft.com/typography/otspec/loca.htm
	 * "Most routines will look at the 'maxp' table to determine the number of glyphs in the font, 
	 * but the value in the 'loca' table must agree."
	 */
	if (!useLongTableVersion && pTableRecord->length != sizeof(USHORT)*(static_cast<size_t>(in_numGlyphs)+1))
		return false;
	if (useLongTableVersion && pTableRecord->length != sizeof(UINT)*(static_cast<size_t>(in_numGlyphs)+1))
		return false;

	TableRecord* pTableRecord_glyf = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('g', 'l', 'y', 'f'));

	if (pTableRecord_glyf == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	assert(sizeof(size_t)>sizeof(in_numGlyphs));
	in_lpTable_loca->offsets.allocate(((size_t) in_numGlyphs)+1);

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
