#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_loca(FILE* fontFile, TableRecord* in_pTableRecord)
{
	assert(in_pTableRecord->tag.uint == CHAR4_TO_UINT_LIL_ENDIAN('l', 'o', 'c', 'a'));

	if (fseek(fontFile, in_pTableRecord->offset, SEEK_SET) != 0)
		return false;

	return true;
}
