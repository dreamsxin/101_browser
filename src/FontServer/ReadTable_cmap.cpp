#include "BasicDataStructures/OffsetofInObject.h"
#include "FontServer/FontServer.h"
#include "BasicDataStructures/Endianess.h"
#include "FontServer/FontServerUtil.h"

bool readTable_cmap(FILE* fontFile, TrueTypeFont* in_trueTypeFont)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('c', 'm', 'a', 'p'));

	if (pTableRecord == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	Table_cmap lTable_cmap;

	if (pTableRecord->length < 2*sizeof(USHORT))
		return false;

	if (fread(&lTable_cmap, 2*sizeof(USHORT), 1, fontFile) != 1)
	{
		return false;
	}

	MTAx::Endianess::switchEndianess(&lTable_cmap.version); // not really necessary since only version 0 is accepted
	MTAx::Endianess::switchEndianess(&lTable_cmap.numTables);

	/*
	* According to
	* http://www.microsoft.com/typography/otspec/cmap.htm
	* the table version number has to be 0.
	*/
	if (lTable_cmap.version != 0)
	{
		return false;
	}

	lTable_cmap.cmapTableEntries.allocate(lTable_cmap.numTables);

	for (size_t i=0; i<lTable_cmap.numTables; i++)
	{
		cmapTableEntry entry;
		if (fread(&entry, sizeof(entry), 1, fontFile) != 1)
		{
			return false;
		}

		MTAx::Endianess::switchEndianess(&entry.platformID);
		MTAx::Endianess::switchEndianess(&entry.encodingID);
		MTAx::Endianess::switchEndianess(&entry.offset);

		printf("platform: %hu\nencoding: %hu\noffset: %u\n\n", 
			entry.platformID, entry.encodingID, entry.offset);

		lTable_cmap.cmapTableEntries.data()[i] = entry;
	}

	for (size_t current_cmapTableEntryIndex = 0; 
		current_cmapTableEntryIndex < lTable_cmap.cmapTableEntries.count();
		current_cmapTableEntryIndex++)
	{
		if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
			return false;
		if (fseek(fontFile, lTable_cmap.cmapTableEntries.data()[current_cmapTableEntryIndex].offset, SEEK_CUR) != 0)
			return false;

		unsigned short format;

		if (fread(&format, sizeof(format), 1, fontFile) != 1)
			return false;

		MTAx::Endianess::switchEndianess(&format);

		printf("format: %hu\n", format);

		switch (format)
		{
		case 0:
			{
				cmapSubTable0 subTable;
				subTable.format = format;
				if (fread(((BYTE*) &subTable)+sizeof(subTable.format), 
					sizeof(subTable)-sizeof(subTable.format), 1, fontFile) != 1)
					return false;

				MTAx::Endianess::switchEndianess(&subTable.length);
				MTAx::Endianess::switchEndianess(&subTable.language);

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
					offsetof_in_object(subTable, endCount)-sizeof(subTable.format), 1, fontFile) != 1)
					return false;

				MTAx::Endianess::switchEndianess(&subTable.length);
				MTAx::Endianess::switchEndianess(&subTable.language);
				MTAx::Endianess::switchEndianess(&subTable.segCountX2);
				MTAx::Endianess::switchEndianess(&subTable.searchRange);
				MTAx::Endianess::switchEndianess(&subTable.entrySelector);
				MTAx::Endianess::switchEndianess(&subTable.rangeShift);

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
					offsetof_in_object(subTable, endCount)+
					/*
					* 4 comes from endCount, startCount, idDelta, idRangeOffset
					*
					* Note that SHORT has the same size as USHORT (for idDelta)
					*/
					4 * sizeof(USHORT) * segCount + 
					sizeof(subTable.reservedPad);

				/*
				 * the case that the size of glyphIdArray is 0
				 * can really happen. The font file DroidSans.ttf
				 * provides a test case for that.
				 */
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
					MTAx::Endianess::switchEndianess(subTable.endCount     .data()+i);
					MTAx::Endianess::switchEndianess(subTable.startCount   .data()+i);
					MTAx::Endianess::switchEndianess(subTable.idDelta      .data()+i);
					MTAx::Endianess::switchEndianess(subTable.idRangeOffset.data()+i);

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
					MTAx::Endianess::switchEndianess(subTable.glyphIdArray .data()+i);
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
					offsetof_in_object(subTable6, glyphIdArray)-sizeof(subTable6.format), 
					1, fontFile) != 1)
					return false;

				MTAx::Endianess::switchEndianess(&subTable6.length);
				MTAx::Endianess::switchEndianess(&subTable6.language);
				MTAx::Endianess::switchEndianess(&subTable6.firstCode);
				MTAx::Endianess::switchEndianess(&subTable6.entryCount);

				/*
				* Since the subTable6.length is sometimes larger than the number of bytes
				* we only check for smaller
				*/
				if (subTable6.length < offsetof_in_object(subTable6, glyphIdArray)+2*subTable6.entryCount)
					return false;
				else if (subTable6.length > offsetof_in_object(subTable6, glyphIdArray)+2*subTable6.entryCount)
				{
					printf("Warning: subTable6.length = %hu, but only %hu bytes necessary\n", 
						subTable6.length, offsetof_in_object(subTable6, glyphIdArray)+2*subTable6.entryCount);
				}

				subTable6.glyphIdArray.allocate(subTable6.entryCount);

				if (fread(subTable6.glyphIdArray.data(), sizeof(USHORT), 
					subTable6.entryCount, fontFile) != subTable6.entryCount)
				{
					subTable6.glyphIdArray.free();
					return false;
				}

				printf("format: %hu\n"
					"length: %hu\n"
					"language: %hu\n"
					"firstCode: %hu\n"
					"entryCount: %hu\n"
					"\n", 
					subTable6.format, subTable6.length, subTable6.language, 
					subTable6.firstCode, subTable6.entryCount);

				subTable6.glyphIdArray.free();
			}
			break;
			/*
			 * The file DroidSansFallback.ttf provides a test case for format
			 */
		case 12:
			{

			}
			break;
		default:
			break;
		}
	}

	printf("\n");

	return true;
}
