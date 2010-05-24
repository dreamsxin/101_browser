#include "FontServer/FontServer.h"
#include <cstdio>
#include <cstring>
#include <cassert>

using namespace std;

/*!
 * Returns true if the checksum of the table by in_pTableDirectory
 * is correct.
 * Otherwise returns false.
 */
bool verifyCheckSum(FILE* fontFile, TableDirectory* in_pTableDirectory)
{
	bool headAdjustment = (in_pTableDirectory->tag.uint == CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));

	unsigned int size = ((in_pTableDirectory->length+3)& ~3)/4;
	fseek(fontFile, in_pTableDirectory->offset, SEEK_SET);
	unsigned int sum=0;

	for (size_t i=0; i<size; i++)
	{
		if (i==2 && headAdjustment)
		{
			if (fseek(fontFile, 4, SEEK_CUR) != 0)
			{
				return false;
			}
		}
		else
		{
			unsigned int currentDword;

			/*
			 * This means the file was probably not large enough for the given size.
			 * So we have an invalid checksum (this error is recoverable - that is why
			 * we return false and don't call exit)
			 */
			if (fread(&currentDword, sizeof(currentDword), 1, fontFile) != 1)
			{
				return false;
			}

			sum+=convertEndianess(currentDword);
		}
	}

	return sum == in_pTableDirectory->checkSum;
}

bool read_cmapTable(FILE* fontFile, TableDirectory* in_pTableDirectory)
{
	fseek(fontFile, in_pTableDirectory->offset, SEEK_SET);

	cmapTable lcmapTable;

	if (fread(&lcmapTable, 2*sizeof(unsigned short), 1, fontFile) != 1)
	{
		return false;
	}

	switchEndianess(&lcmapTable.version); // not really necessary since only version 0 is accepted
	switchEndianess(&lcmapTable.numTables);

	/*
	 * According to
	 * http://www.microsoft.com/typography/otspec/cmap.htm
	 * the table version number has to be 0.
	 */
	if (lcmapTable.version != 0)
	{
		return false;
	}

	lcmapTable.cmapTableEntries.reserve(lcmapTable.numTables);

	for (size_t i=0; i<lcmapTable.numTables; i++)
	{
		cmapTableEntry entry;
		if (fread(&entry, sizeof(entry), 1, fontFile) != 1)
		{
			return false;
		}

		switchEndianess(&entry.platformID);
		switchEndianess(&entry.encodingID);
		switchEndianess(&entry.offset);

		printf("platform: %hu\nencoding: %hu\noffset: %u\n\n", 
			entry.platformID, entry.encodingID, entry.offset);

		lcmapTable.cmapTableEntries.push_back(entry);
	}

	return true;
}


int readTTF(char* filename) {
	FILE* fontFile = fopen(filename, "rb");

	if (!fontFile)
		return -1;

	TrueTypeFont font;

	if (fread(&font.offsetTable, sizeof(font.offsetTable), 1, fontFile)!=1)
		return -1;

	switchEndianess(&font.offsetTable.numTables);
	switchEndianess(&font.offsetTable.searchRange);
	switchEndianess(&font.offsetTable.entrySelector);
	switchEndianess(&font.offsetTable.rangeShift);

	if (memcmp(&font.offsetTable.sfntVersion, csfntVersion, 4))
		return -2;	
	if (font.offsetTable.searchRange != 16*1<<floorLog2(font.offsetTable.numTables))
		return -2;
	if (font.offsetTable.entrySelector != floorLog2(font.offsetTable.numTables))
		return -2;
	if (font.offsetTable.rangeShift != font.offsetTable.numTables*16-font.offsetTable.searchRange)
		return -2;

	font.tableDirectories.reserve(font.offsetTable.numTables);

	for (size_t i=0; i<font.offsetTable.numTables; i++)
	{
		TableDirectory tableDirectory;

		if (fread(&tableDirectory, sizeof(tableDirectory), 1, fontFile) != 1)
			return -1;

		switchEndianess(&tableDirectory.checkSum);
		switchEndianess(&tableDirectory.offset);
		switchEndianess(&tableDirectory.length);

		/*
		 * In http://www.microsoft.com/typography/otspec/otff.htm
		 * section "Organization of an OpenType Font" we can read:
		 * "Entries in the Table Record must be sorted in ascending order by tag."
		 * This is what we test here
		 */
		if (i>0 && memcmp(&font.tableDirectories.at(i-1).tag, &tableDirectory.tag, 4) >= 0)
			return -3;

		font.tableDirectories.push_back(tableDirectory);
	}

	for (vector<TableDirectory>::iterator i=font.tableDirectories.begin(); i!=font.tableDirectories.end(); ++i)
	{
		printf("Table:\t%c%c%c%c\n", i->tag.bytes[0], i->tag.bytes[1], i->tag.bytes[2], i->tag.bytes[3]);

		if (!verifyCheckSum(fontFile, &*i))
		{
			return -4;
		}

		switch (i->tag.uint) {
			case CHAR4_TO_UINT_LIL_ENDIAN('c', 'm', 'a', 'p'):
				if (!read_cmapTable(fontFile, &*i))
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
