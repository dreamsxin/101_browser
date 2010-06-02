#include "FontServer/FontServer.h"
#include <cstdio>
#include <cstring>
#include <cassert>


int readTTF(char* filename) {
	FILE* fontFile = fopen(filename, "rb");

	if (!fontFile)
		return -1;

	OffsetTable offsetTable;

	if (!readOffsetTable(fontFile, &offsetTable))
		return -1;

	TrueTypeFont font;
	font.tableDirectories.allocate(offsetTable.numTables);

	for (size_t i=0; i<offsetTable.numTables; i++)
	{
		TableDirectory tableDirectory;

		if (!readTableDirectory(fontFile, &tableDirectory))
			return -1;

		/*
		* In http://www.microsoft.com/typography/otspec/otff.htm
		* section "Organization of an OpenType Font" we can read:
		* "Entries in the Table Record must be sorted in ascending order by tag."
		* This is what we test here
		*/
		if (i>0 && memcmp(&font.tableDirectories.data()[i-1].tag, &tableDirectory.tag, 4) >= 0)
			return -3;

		font.tableDirectories.data()[i] = tableDirectory;
	}

	for (size_t currentTableDirectoryIndex = 0; 
		currentTableDirectoryIndex < font.tableDirectories.count();
		currentTableDirectoryIndex++)
	{
		TableDirectory* pCurrentTableDirectory = font.tableDirectories.data()+currentTableDirectoryIndex;

		printf("Table:\t%c%c%c%c\n", 
			pCurrentTableDirectory->tag.bytes[0], 
			pCurrentTableDirectory->tag.bytes[1], 
			pCurrentTableDirectory->tag.bytes[2], 
			pCurrentTableDirectory->tag.bytes[3]);

		if (!verifyCheckSum(fontFile, pCurrentTableDirectory))
		{
			return -4;
		}

		switch (pCurrentTableDirectory->tag.uint) {
			case CHAR4_TO_UINT_LIL_ENDIAN('c', 'm', 'a', 'p'):
				if (!readTable_cmap(fontFile, pCurrentTableDirectory))
					return -5;
				break;
			case CHAR4_TO_UINT_LIL_ENDIAN('g', 'l', 'y', 'f'):
				break;
			default:
				break;
		}
	}

	fclose(fontFile);

	return 0;
}
