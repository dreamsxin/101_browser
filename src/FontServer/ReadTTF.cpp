#include "FontServer/FontServer.h"
#include <cstdio>
#include <cstring>

int readTTF(char* filename) {
	FILE* fontFile = fopen(filename, "rb");

	if (!fontFile)
		return -1;

	OffsetTable offsetTable;

	if (!readOffsetTable(fontFile, &offsetTable))
	{
		fclose(fontFile);
		return -1;
	}

	TrueTypeFont font;
	font.tableDirectory.allocate(offsetTable.numTables);

	for (size_t i=0; i<offsetTable.numTables; i++)
	{
		TableRecord TableRecord;

		if (!readTableRecord(fontFile, &TableRecord))
		{
			font.tableDirectory.free();
			fclose(fontFile);
			return -1;
		}

		/*
		* In http://www.microsoft.com/typography/otspec/otff.htm
		* section "Organization of an OpenType Font" we can read:
		* "Entries in the Table Record must be sorted in ascending order by tag."
		* This is what we test here
		*/
		if (i>0 && memcmp(&font.tableDirectory.data()[i-1].tag, &TableRecord.tag, 4) >= 0)
		{
			font.tableDirectory.free();
			fclose(fontFile);
			return -3;
		}

		if (!verifyCheckSum(fontFile, &TableRecord))
		{
			font.tableDirectory.free();
			fclose(fontFile);
			return -4;
		}

		font.tableDirectory.data()[i] = TableRecord;
	}

	for (size_t currentTableRecordIndex = 0; 
		currentTableRecordIndex < font.tableDirectory.count();
		currentTableRecordIndex++)
	{
		TableRecord* pCurrentTableRecord = font.tableDirectory.data()+currentTableRecordIndex;

		printf("Table:\t%c%c%c%c\n", 
			pCurrentTableRecord->tag.bytes[0], 
			pCurrentTableRecord->tag.bytes[1], 
			pCurrentTableRecord->tag.bytes[2], 
			pCurrentTableRecord->tag.bytes[3]);
	}

	printf("\n");

	if (!readTable_head(fontFile, &font))
	{
		font.tableDirectory.free();
		fclose(fontFile);
		return -6;
	}

	if (!readTable_cmap(fontFile, &font))
	{
		font.tableDirectory.free();
		fclose(fontFile);
		return -5;
	}

	if (!readTable_glyf(fontFile, &font))
	{
		font.tableDirectory.free();
		fclose(fontFile);
		return -6;
	}

	fclose(fontFile);

	return 0;
}
