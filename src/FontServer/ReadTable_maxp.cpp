#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_maxp(FILE* fontFile, TrueTypeFont* in_trueTypeFont, Table_maxp* in_pTable_maxp)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('m', 'a', 'x', 'p'));
	
	if (pTableRecord == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	if (pTableRecord->length < sizeof(Fixed)+sizeof(USHORT)) // Size of the table with 
		                                                     // version number 0.5
	{
		return false;
	}

	if (fread(in_pTable_maxp, sizeof(Fixed)+sizeof(USHORT), 1, fontFile) != 1)
	{
		return false;
	}

	switchEndianess(&in_pTable_maxp->tableVersionNumber.beforeComma);
	switchEndianess(&in_pTable_maxp->tableVersionNumber. afterComma);
	switchEndianess(&in_pTable_maxp->numGlyphs);

	if (in_pTable_maxp->tableVersionNumber.beforeComma == 0 || 
		in_pTable_maxp->tableVersionNumber.afterComma == 5)
	{
		if (pTableRecord->length != sizeof(Fixed)+sizeof(USHORT))
			return false;
	}
	else if (in_pTable_maxp->tableVersionNumber.beforeComma == 1 && 
		in_pTable_maxp->tableVersionNumber.afterComma == 0)
	{
		if (pTableRecord->length != sizeof(Table_maxp))
			return false;

		if (fread(&in_pTable_maxp->maxPoints, 
			sizeof(Table_maxp)-sizeof(Fixed)-sizeof(USHORT), 
			1, fontFile) != 1)
			return false;

		switchEndianess(&in_pTable_maxp->maxPoints);
		switchEndianess(&in_pTable_maxp->maxContours);
		switchEndianess(&in_pTable_maxp->maxCompositePoints);
		switchEndianess(&in_pTable_maxp->maxCompositeContours);
		switchEndianess(&in_pTable_maxp->maxZones);
		switchEndianess(&in_pTable_maxp->maxTwilightPoints);
		switchEndianess(&in_pTable_maxp->maxStorage);
		switchEndianess(&in_pTable_maxp->maxFunctionDefs);
		switchEndianess(&in_pTable_maxp->maxInstructionDefs);
		switchEndianess(&in_pTable_maxp->maxStackElements);
		switchEndianess(&in_pTable_maxp->maxSizeOfInstructions);
		switchEndianess(&in_pTable_maxp->maxComponentElements);
		switchEndianess(&in_pTable_maxp->maxComponentDepth);
	}
	else
	{
		return false;
	}

	printf(
		"tableVersionNumber: %hi.%hu\n"
		"numGlyphs: %hu\n", 
		in_pTable_maxp->tableVersionNumber.beforeComma, 
		in_pTable_maxp->tableVersionNumber. afterComma,
		in_pTable_maxp->numGlyphs);

	if (in_pTable_maxp->tableVersionNumber.beforeComma == 1 && 
		in_pTable_maxp->tableVersionNumber.afterComma == 0)
	{
		printf(
			"maxPoints: %hu\n"
			"maxContours: %hu\n"
			"maxCompositePoints: %hu\n"
			"maxCompositeContours: %hu\n"
			"maxZones: %hu\n"
			"maxTwilightPoints: %hu\n"
			"maxStorage: %hu\n"
			"maxFunctionDefs: %hu\n"
			"maxInstructionDefs: %hu\n"
			"maxStackElements: %hu\n"
			"maxSizeOfInstructions: %hu\n"
			"maxComponentElements: %hu\n"
			"maxComponentDepth: %hu\n",
			in_pTable_maxp->maxPoints,
			in_pTable_maxp->maxContours,
			in_pTable_maxp->maxCompositePoints,
			in_pTable_maxp->maxCompositeContours,
			in_pTable_maxp->maxZones,
			in_pTable_maxp->maxTwilightPoints,
			in_pTable_maxp->maxStorage,
			in_pTable_maxp->maxFunctionDefs,
			in_pTable_maxp->maxInstructionDefs,
			in_pTable_maxp->maxStackElements,
			in_pTable_maxp->maxSizeOfInstructions,
			in_pTable_maxp->maxComponentElements,
			in_pTable_maxp->maxComponentDepth);
	}
	
	printf("\n");

	return true;
}
