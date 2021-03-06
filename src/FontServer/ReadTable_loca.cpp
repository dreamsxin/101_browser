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

#include <cassert>
#include "MiniStdlib/climits.h"
#include "FontServer/FontServer.h"
#include "BasicDataStructures/Endianess.h"

bool readTable_loca(FILE* fontFile, TrueTypeFont* in_trueTypeFont, 
					bool useLongTableVersion, Table_loca *in_lpTable_loca)
{

	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('l', 'o', 'c', 'a'));
	
	if (pTableRecord == NULL)
		return false;

	TableRecord* pTableRecord_glyf = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('g', 'l', 'y', 'f'));

	if (pTableRecord_glyf == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	/*
	 * This variable has to equal the value numGlyphs from the maxp table.
	 * At the moment this won't be checked for a more simple implementation.
	 */
	size_t numGlyphs;

	/*
	 * The file DroidSans.ttf provides a test case for
	 * the short table version
	 */
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
		 * "In the particular case of the last glyph(s), loca[n] will be equal 
		 * the length of the glyph data ('glyf') table."
		 * Because of the former condition the glyf table can't be empty. So there
		 * has to be a second entry different from the first and pTableRecord->length 
		 * has to be 4 or larger.
		 */
		if ((pTableRecord->length & 0x1) != 0 || pTableRecord->length <= 4)
			return false;
		else
			numGlyphs = pTableRecord->length/2-1;
	}
	/*
	 * The file DroidSansFallback.ttf provides a test case for
	 * the long table version
	 */
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
		 * "In the particular case of the last glyph(s), loca[n] will be equal 
		 * the length of the glyph data ('glyf') table."
		 * Because of the former condition the glyf table can't be empty. So there
		 * has to be a second entry different from the first and pTableRecord->length 
		 * has to be 8 or larger.
		 */
		if ((pTableRecord->length & 0x3) != 0 || pTableRecord->length <=8)
			return false;
		else
			numGlyphs = pTableRecord->length/4-1;
	}

	/*
	 * This assertion is true because when we compute the
	 * value of numGlyfs we either divide an UINT (for which 
	 * sizeof(UINT) is less or equal sizeof(size_t)) value by
	 * 2 or 4 -- so it has to be < SIZE_MAX - 1 (even smaller --
	 * but we won't need more exactness ;-) )
	 */
	assert(numGlyphs < SIZE_MAX - 1);
	in_lpTable_loca->offsets.allocate(numGlyphs+1);

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

			MTAx::Endianess::switchEndianess(&readLongOffset);

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

			MTAx::Endianess::switchEndianess(&readShortOffset);

			readOffset = 2*static_cast<UINT>(readShortOffset);
		}

		/*
		 * According to
		 * http://www.microsoft.com/typography/otspec/loca.htm
		 * "The offsets must be in ascending order with loca[n] <= loca[n+1]."
		 */
		if (previousOffset > readOffset && readOffset != 0)
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

#if 0
	for (size_t currentIndex = 0; currentIndex < in_lpTable_loca->offsets.count(); currentIndex++)
	{
		printf("%lu: %lu\n", currentIndex, in_lpTable_loca->offsets.data()[currentIndex]);
	}

	printf("\n");
#endif

	return true;
}
