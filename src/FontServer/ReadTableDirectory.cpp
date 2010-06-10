#include "FontServer/FontServer.h"
#include "BasicDataStructures/Endianess.h"

bool readTableRecord(FILE* fontFile, TableRecord* in_pTableRecord)
{
	if (fread(in_pTableRecord, sizeof(TableRecord), 1, fontFile) != 1)
		return false;

	MTAx::Endianess::switchEndianess(&in_pTableRecord->checkSum);
	MTAx::Endianess::switchEndianess(&in_pTableRecord->offset  );
	MTAx::Endianess::switchEndianess(&in_pTableRecord->length  );

	return true;
}
