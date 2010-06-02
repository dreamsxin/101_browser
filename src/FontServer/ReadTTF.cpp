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
	if (fseek(fontFile, in_pTableDirectory->offset, SEEK_SET) != 0)
		return false;

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

	lcmapTable.cmapTableEntries.allocate(lcmapTable.numTables);

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

		lcmapTable.cmapTableEntries.data()[i] = entry;
	}

	for (size_t current_cmapTableEntryIndex = 0; 
		current_cmapTableEntryIndex < lcmapTable.cmapTableEntries.count();
		current_cmapTableEntryIndex++)
	{
		if (fseek(fontFile, in_pTableDirectory->offset, SEEK_SET) != 0)
			return false;
		if (fseek(fontFile, lcmapTable.cmapTableEntries.data()[current_cmapTableEntryIndex].offset, SEEK_CUR) != 0)
			return false;

		unsigned short format;

		if (fread(&format, sizeof(format), 1, fontFile) != 1)
			return false;

		switchEndianess(&format);

		switch (format)
		{
		case 0:
			{
				cmapSubTable0 subTable;
				subTable.format = format;
				if (fread(((BYTE*) &subTable)+sizeof(subTable.format), 
					sizeof(subTable)-sizeof(subTable.format), 1, fontFile) != 1)
					return false;

				switchEndianess(&subTable.length);
				switchEndianess(&subTable.language);

				if (subTable.length != sizeof(cmapSubTable0))
					return false;

				printf("format: %hu\nlength: %hu\nlanguage: %hu\n\n", 
					subTable.format, subTable.length, subTable.language);
			}
			break;
		case 4:
			{
				cmapSubTable4 subTable;
				subTable.format = format;
				if (fread(((BYTE*) &subTable)+sizeof(subTable.format), 
					offsetof(cmapSubTable4, endCount)-sizeof(subTable.format), 1, fontFile) != 1)
					return false;

				switchEndianess(&subTable.length);
				switchEndianess(&subTable.language);
				switchEndianess(&subTable.segCountX2);
				switchEndianess(&subTable.searchRange);
				switchEndianess(&subTable.entrySelector);
				switchEndianess(&subTable.rangeShift);

				/*
				* segCountX2 is  2 x segCount. 
				* So it has to be an even value.
				*/
				if (subTable.segCountX2 % 2 == 1)
					return false;

				USHORT segCount = subTable.segCountX2 / 2;

				// searchRange has to be 2 x (2**floor(log2(segCount)))
				if (subTable.searchRange != 2*(1<<floorLog2(segCount)))
					return false;

				// entrySelector has to be log2(searchRange/2)
				if (subTable.entrySelector != floorLog2(subTable.searchRange/2))
					return false;

				// rangeShift has to be 2 x segCount - searchRange
				assert(2* segCount >= subTable.searchRange);
				if (subTable.rangeShift != 2* segCount - subTable.searchRange)
					return false;

				subTable.endCount     .allocate(segCount);
				subTable.startCount   .allocate(segCount);
				subTable.idDelta      .allocate(segCount);
				subTable.idRangeOffset.allocate(segCount);

				size_t sizeWithoutGlyphIdArray = 
					offsetof(cmapSubTable4, endCount)+
					/*
					* 4 comes from endCount, startCount, idDelta, idRangeOffset
					*
					* Note that SHORT has the same size as USHORT (for idDelta)
					*/
					4 * sizeof(USHORT) * segCount + 
					sizeof(subTable.reservedPad);

				if (sizeWithoutGlyphIdArray > subTable.length)
					return false;

				size_t glyphIdArraySize = subTable.length - sizeWithoutGlyphIdArray;

				/*
				* since it is an array of USHORTs we need an 
				* even number of bytes
				*/
				if (glyphIdArraySize % sizeof(USHORT) == 1)
					return false;

				subTable.glyphIdArray.allocate(glyphIdArraySize);

				if (
					fread(subTable.endCount     .data(), sizeof(USHORT), subTable.endCount     .count(), fontFile) != subTable.endCount     .count() ||
					fread(&subTable.reservedPad, sizeof(USHORT), 1, fontFile) != 1 ||
					fread(subTable.startCount   .data(), sizeof(USHORT), subTable.startCount   .count(), fontFile) != subTable.startCount   .count() ||
					fread(subTable.idDelta      .data(), sizeof( SHORT), subTable.idDelta      .count(), fontFile) != subTable.idDelta      .count() ||
					fread(subTable.idRangeOffset.data(), sizeof(USHORT), subTable.idRangeOffset.count(), fontFile) != subTable.idRangeOffset.count() ||
					fread(subTable.glyphIdArray .data(), sizeof(USHORT), subTable.glyphIdArray .count(), fontFile) != subTable.glyphIdArray .count()
					)
				{
					subTable.endCount     .free();
					subTable.startCount   .free();
					subTable.idDelta      .free();
					subTable.idRangeOffset.free();
					subTable.glyphIdArray .free();

					return false;
				}

				if (subTable.reservedPad != 0)
				{
					subTable.endCount     .free();
					subTable.startCount   .free();
					subTable.idDelta      .free();
					subTable.idRangeOffset.free();
					subTable.glyphIdArray .free();

					return false;
				}

				for (size_t i=0; i<segCount; i++)
				{
					switchEndianess(subTable.endCount     .data()+i);
					switchEndianess(subTable.startCount   .data()+i);
					switchEndianess(subTable.idDelta      .data()+i);
					switchEndianess(subTable.idRangeOffset.data()+i);

					/*
					 * According to 
					 * http://www.microsoft.com/typography/otspec/cmap.htm
					 * Section "Format 4: Segment mapping to delta values":
					 * "The segments are sorted in order of increasing endCode values"
					 */
					if (i>0 && subTable.endCount.data()[i]<=subTable.endCount.data()[i-1])
					{
						return false;
					}
				}

				for (size_t i=0; i<glyphIdArraySize; i++)
				{
					switchEndianess(subTable.glyphIdArray .data()+i);
				}

				printf("format: %hu\nlength: %hu\nlanguage: %hu\nsegCountX2: %hu\nsearchRange: %hu\nentrySelector: %hu\nrangeShift: %hu\n\n", 
					subTable.format, 
					subTable.length, 
					subTable.language,
					subTable.segCountX2,
					subTable.searchRange,
					subTable.entrySelector,
					subTable.rangeShift);
			}
			break;
		case 6:
			{
				cmapSubTable6 subTable6;
				subTable6.format = format;
				if (fread(((BYTE*) &subTable6)+sizeof(subTable6.format), 
					offsetof(cmapSubTable6, glyphIdArray)-sizeof(subTable6.format), 
					1, fontFile) != 1)
					return false;

				switchEndianess(&subTable6.length);
				switchEndianess(&subTable6.language);
				switchEndianess(&subTable6.firstCode);
				switchEndianess(&subTable6.entryCount);

				/*
				* Since the subTable6.length is sometimes larger than the number of bytes
				* we only check for smaller
				*/
				if (subTable6.length < offsetof(cmapSubTable6, glyphIdArray)+2*subTable6.entryCount)
					return false;
				else if (subTable6.length > offsetof(cmapSubTable6, glyphIdArray)+2*subTable6.entryCount)
				{
					printf("Warning: subTable6.length = %hu, but only %hu bytes necessary\n", 
						subTable6.length, offsetof(cmapSubTable6, glyphIdArray)+2*subTable6.entryCount);
				}

				subTable6.glyphIdArray.allocate(subTable6.entryCount);

				if (fread(subTable6.glyphIdArray.data(), sizeof(USHORT), 
					subTable6.entryCount, fontFile) != subTable6.entryCount)
				{
					subTable6.glyphIdArray.free();
					return false;
				}

				printf("format: %hu\nlength: %hu\nlanguage: %hu\nfirstCode: %hu\nentryCount: %hu\n\n", 
					subTable6.format, subTable6.length, subTable6.language, 
					subTable6.firstCode, subTable6.entryCount);

				subTable6.glyphIdArray.free();
			}
			break;
		default:
			printf("format: %hu\n\n", format);
			break;
		}


	}

	return true;
}

bool readOffsetTable(FILE* fontFile, OffsetTable* in_pOffsetTable)
{
	if (fread(in_pOffsetTable, sizeof(OffsetTable), 1, fontFile)!=1)
		return false;

	switchEndianess(&in_pOffsetTable->numTables);
	switchEndianess(&in_pOffsetTable->searchRange);
	switchEndianess(&in_pOffsetTable->entrySelector);
	switchEndianess(&in_pOffsetTable->rangeShift);

	if (memcmp(&in_pOffsetTable->sfntVersion, csfntVersion, 4))
		return false;	
	if (in_pOffsetTable->searchRange != 16*1<<floorLog2(in_pOffsetTable->numTables))
		return false;
	if (in_pOffsetTable->entrySelector != floorLog2(in_pOffsetTable->numTables))
		return false;
	if (in_pOffsetTable->rangeShift != in_pOffsetTable->numTables*16-in_pOffsetTable->searchRange)
		return false;

	return true;
}

bool readTableDirectory(FILE* fontFile, TableDirectory* in_pTableDirectory)
{
	if (fread(in_pTableDirectory, sizeof(TableDirectory), 1, fontFile) != 1)
		return false;

	switchEndianess(&in_pTableDirectory->checkSum);
	switchEndianess(&in_pTableDirectory->offset);
	switchEndianess(&in_pTableDirectory->length);

	return true;
}

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
				if (!read_cmapTable(fontFile, pCurrentTableDirectory))
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
