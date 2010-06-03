#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

TableRecord* getTableRecordPointer(FILE* fontFile, const ArrayBlock<TableRecord>* in_pTableDirectory, UINT in_tag)
{
	for (size_t currentTableRecordIndex = 0; 
		currentTableRecordIndex < in_pTableDirectory->count();
		currentTableRecordIndex++)
	{
		TableRecord* pCurrentTableRecord = in_pTableDirectory->data()+currentTableRecordIndex;

		if (pCurrentTableRecord->tag.uint == in_tag)
		{
			return pCurrentTableRecord;
		}
	}

	return NULL;
}
