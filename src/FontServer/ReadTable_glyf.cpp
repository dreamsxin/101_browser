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
#include "FontServer/FontServer.h"
#include "BasicDataStructures/Endianess.h"

bool readTable_glyf(FILE* fontFile, TrueTypeFont* in_trueTypeFont)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('g', 'l', 'y', 'f'));

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	/*
	 * The 5 shorts, whose length we compare to, are - of course:
	 * numberOfContours, xMin, yMin, xMax, yMax
	 */
	if (pTableRecord->length < 5*sizeof(SHORT))
		return false;

	Table_glyf lTable_glyf;

	if (fread(&lTable_glyf, 5*sizeof(SHORT), 1, fontFile) != 1)
	{
		return false;
	}

	MTAx::Endianess::switchEndianess(&lTable_glyf.numberOfContours);
	MTAx::Endianess::switchEndianess(&lTable_glyf.xMin);
	MTAx::Endianess::switchEndianess(&lTable_glyf.yMin);
	MTAx::Endianess::switchEndianess(&lTable_glyf.xMax);
	MTAx::Endianess::switchEndianess(&lTable_glyf.yMax);

	printf("numberOfContours: %hi\nxMin: %hi\nyMin: %hi\nxMax: %hi\nyMax %hi\n\n", 
			lTable_glyf.numberOfContours, 
			lTable_glyf.xMin, 
			lTable_glyf.yMin,
			lTable_glyf.xMax,
			lTable_glyf.yMax);

	return true;
}
