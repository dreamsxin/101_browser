#include "FontServer/FontServer.h"
#include <cstdio>
#include <cstring>
#include <cassert>

using namespace std;

#define CHAR4_TO_UINT_LIL_ENDIAN(a, b, c, d) (((unsigned char) a)+((unsigned char) b)*(1<<8)+((unsigned char) c)*(1<<16)+((unsigned char) d)*(1<<24)) 

/*!
If we compute the checksum of the head table we have to ignore the 3rd DWORD.
In this case we have to set 'headAdjustment' to true. In any other case it should be 'false'
 */
unsigned int computeCheckSum(const ArrayBlock<unsigned char>& actTable, const bool headAdjustment=false)
{
	unsigned int sum=0;

	// The size has to be dividable by 4
	assert((actTable.count()& 0x3) == 0);

	unsigned int* actPos=(unsigned int*) actTable.data();
	unsigned int* endPos=actPos+actTable.count()/4;

	for (size_t i=0; i<actTable.count()/4; i++)
	{
		if (headAdjustment && i==2)
		{
			actPos++;
			continue;
		}

		sum+=convertEndianess(*actPos++);
	}

	return sum;
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

	unsigned char correctVersion[4]={0x00, 0x01, 0x00, 0x00};

	if (memcmp(&font.offsetTable.sfntVersion, correctVersion, 4)) return -2;	
	if (font.offsetTable.searchRange!=16*1<<floorLog2(font.offsetTable.numTables)) return -2;
	if (font.offsetTable.entrySelector!=floorLog2(font.offsetTable.numTables)) return -2;
	if (font.offsetTable.rangeShift!=font.offsetTable.numTables*16-font.offsetTable.searchRange) return -2;

	font.tableDirectories.reserve(font.offsetTable.numTables);

	for (size_t i=0; i<font.offsetTable.numTables; i++)
	{
		TableDirectory tableDirectory;

		if (fread(&tableDirectory, sizeof(tableDirectory), 1, fontFile)!=1)
			return -1;

		switchEndianess(&tableDirectory.checkSum);
		switchEndianess(&tableDirectory.offset);
		switchEndianess(&tableDirectory.length);

		if (i>0 && memcmp(&font.tableDirectories.at(i-1).tag, &tableDirectory.tag, 4)>=0)
			return -3;

		font.tableDirectories.push_back(tableDirectory);
	}

	for (vector<TableDirectory>::iterator i=font.tableDirectories.begin(); i!=font.tableDirectories.end(); ++i)
	{
		printf("Table:\t%c%c%c%c\n", i->tag.bytes[0], i->tag.bytes[1], i->tag.bytes[2], i->tag.bytes[3]);
		
		ArrayBlock<unsigned char> actTable;
		actTable.allocate((i->length+3)& ~3);

		fseek(fontFile, i->offset, SEEK_SET);
		if (fread(actTable.data(), actTable.count(), 1, fontFile) != 1)
			return -4;

		bool headAdjustment = (i->tag.uint == CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));

		if (computeCheckSum(actTable, headAdjustment)!=i->checkSum)
		{
			return -4;
		}

		actTable.free();

		switch (i->tag.uint) {
			case CHAR4_TO_UINT_LIL_ENDIAN('c', 'm', 'a', 'p'):
				//printf("cmap found\n");
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
