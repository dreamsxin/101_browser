#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

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