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
