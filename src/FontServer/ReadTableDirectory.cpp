#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTableRecord(FILE* fontFile, TableRecord* in_pTableRecord)
{
	if (fread(in_pTableRecord, sizeof(TableRecord), 1, fontFile) != 1)
		return false;

	switchEndianess(&in_pTableRecord->checkSum);
	switchEndianess(&in_pTableRecord->offset);
	switchEndianess(&in_pTableRecord->length);

	return true;
}
