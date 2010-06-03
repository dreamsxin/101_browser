#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable(FILE* fontFile, const ArrayBlock<TableRecord>* in_pTableDirectory, UINT in_tag)
{
	for (size_t currentTableRecordIndex = 0; 
		currentTableRecordIndex < in_pTableDirectory->count();
		currentTableRecordIndex++)
	{
		TableRecord* pCurrentTableRecord = in_pTableDirectory->data()+currentTableRecordIndex;

		if (pCurrentTableRecord->tag.uint == in_tag)
		{
			switch (in_tag)
			{
			case CHAR4_TO_UINT_LIL_ENDIAN('c', 'm', 'a', 'p'):
				return readTable_cmap(fontFile, pCurrentTableRecord);
			case CHAR4_TO_UINT_LIL_ENDIAN('g', 'l', 'y', 'f'):
				return readTable_glyf(fontFile, pCurrentTableRecord);
			default:
				return false;
			}
		}
	}

	return false;
}
