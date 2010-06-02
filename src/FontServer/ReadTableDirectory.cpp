#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTableDirectory(FILE* fontFile, TableDirectory* in_pTableDirectory)
{
	if (fread(in_pTableDirectory, sizeof(TableDirectory), 1, fontFile) != 1)
		return false;

	switchEndianess(&in_pTableDirectory->checkSum);
	switchEndianess(&in_pTableDirectory->offset);
	switchEndianess(&in_pTableDirectory->length);

	return true;
}
