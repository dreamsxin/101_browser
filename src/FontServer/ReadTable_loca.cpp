#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_loca(FILE* fontFile, TrueTypeFont* in_trueTypeFont, 
					USHORT in_numGlyphs, bool useLongTableVersion)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('l', 'o', 'c', 'a'));
	
	if (pTableRecord == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	if (useLongTableVersion)
	{
		Table_loca_long lTable_loca;
		lTable_loca.offsets.allocate(in_numGlyphs+1);

		if (fread(lTable_loca.offsets.data(), sizeof(UINT), in_numGlyphs+1, fontFile) != in_numGlyphs+1)
		{
			lTable_loca.offsets.free();
			return false;
		}

		lTable_loca.offsets.free();
	}
	else
	{
		Table_loca_short lTable_loca;
		lTable_loca.offsets.allocate(in_numGlyphs+1);

		if (fread(lTable_loca.offsets.data(), sizeof(USHORT), in_numGlyphs+1, fontFile) != in_numGlyphs+1)
		{
			lTable_loca.offsets.free();
			return false;
		}

		lTable_loca.offsets.free();
	}

	return true;
}
