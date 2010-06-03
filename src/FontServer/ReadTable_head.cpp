#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_head(FILE* fontFile, TableRecord* in_pTableRecord)
{
	assert(in_pTableRecord->tag.uint == CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));

	if (fseek(fontFile, in_pTableRecord->offset, SEEK_SET) != 0)
		return false;

	return true;
}