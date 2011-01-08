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

#include "FontServer/FontServer.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include <cstring>

int readTTF(char* filename) {
	FILE* fontFile = MTAx_fopen(filename, "rb");

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

	Table_head lTable_head;

	if (!readTable_head(fontFile, &font, &lTable_head))
	{
		font.tableDirectory.free();
		fclose(fontFile);
		return -5;
	}

	if (!readTable_cmap(fontFile, &font))
	{
		font.tableDirectory.free();
		fclose(fontFile);
		return -6;
	}

	Table_loca lTable_loca;

	if (!readTable_loca(fontFile, &font, 
		lTable_head.indexToLocFormat == 1, &lTable_loca))
	{
		lTable_loca.offsets.free();
		font.tableDirectory.free();
		fclose(fontFile);
		return -8;
	}

	if (!readTable_glyf(fontFile, &font))
	{
		lTable_loca.offsets.free();
		font.tableDirectory.free();
		fclose(fontFile);
		return -9;
	}

	fclose(fontFile);

	return 0;
}
