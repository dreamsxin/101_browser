#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_head(FILE* fontFile, TrueTypeFont* in_trueTypeFont, Table_head* lpTable_head)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));
	
	if (pTableRecord == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	if (pTableRecord->length != sizeof(Table_head))
		return false;

	if (fread(lpTable_head, sizeof(Table_head), 1, fontFile) != 1)
	{
		return false;
	}

	switchEndianess(&lpTable_head->tableVersionNumber.beforeComma);
	switchEndianess(&lpTable_head->tableVersionNumber. afterComma);
	switchEndianess(&lpTable_head->fontRevision      .beforeComma);
	switchEndianess(&lpTable_head->fontRevision      . afterComma);
	switchEndianess(&lpTable_head->checkSumAdjustment            );
	switchEndianess(&lpTable_head->magicNumber                   );
	switchEndianess(&lpTable_head->flags                         );
	switchEndianess(&lpTable_head->unitsPerEm                    );
	switchEndianess(&lpTable_head->created                       );
	switchEndianess(&lpTable_head->modified                      );
	switchEndianess(&lpTable_head->xMin                          );
	switchEndianess(&lpTable_head->yMin                          );
	switchEndianess(&lpTable_head->xMax                          );
	switchEndianess(&lpTable_head->xMax                          );
	switchEndianess(&lpTable_head->macStyle                      );
	switchEndianess(&lpTable_head->lowestRecPPEM                 );
	switchEndianess(&lpTable_head->fontDirectionHint             );
	switchEndianess(&lpTable_head->indexToLocFormat              );
	switchEndianess(&lpTable_head->glyphDataFormat               );

	/*
	 * According to http://www.microsoft.com/typography/otspec/head.htm
	 * this field must have the value 0x5F0F3CF5
	 */
	if (lpTable_head->magicNumber != 0x5F0F3CF5)
		return false;

	/*
	 * According to http://www.microsoft.com/typography/otspec/head.htm
	 * this field must be from 16 to 16384
	 */
	if (lpTable_head->unitsPerEm < 16 || lpTable_head->unitsPerEm > 16384)
		return false;

	if (lpTable_head->glyphDataFormat != 0)
		return false;

	if (lpTable_head->indexToLocFormat != 0 && lpTable_head->indexToLocFormat != 1)
		return false;

	printf("tableVersionNumber: %hi.%hu\n"
		"fontRevision: %hi.%hu\n"
		"checkSumAdjustment: %X\n"
		"magicNumber: %X\n"
		"flags: %hX\n"
		"unitsPerEm: %hu\n"
#ifdef _WIN32
		"created: %I64d\n"
		"modified: %I64d\n"
#endif
		"xMin: %hi\n"
		"yMin: %hi\n"
		"xMax: %hi\n"
		"yMax: %hi\n"
		"macStyle: %hu\n"
		"lowestRecPPEM: %hu\n"
		"fontDirectionHint: %hi\n"
		"indexToLocFormat: %hi\n"
		"glyphDataFormat: %hi\n"
		"\n",
		lpTable_head->tableVersionNumber.beforeComma, 
		lpTable_head->tableVersionNumber. afterComma, 
		lpTable_head->fontRevision      .beforeComma, 
		lpTable_head->fontRevision      . afterComma, 
		lpTable_head->checkSumAdjustment, 
		lpTable_head->magicNumber, 
		lpTable_head->flags, 
		lpTable_head->unitsPerEm, 
#ifdef _WIN32
		lpTable_head->created, 
		lpTable_head->modified, 
#endif
		lpTable_head->xMin, 
		lpTable_head->yMin, 
		lpTable_head->xMax, 
		lpTable_head->xMax, 
		lpTable_head->macStyle, 
		lpTable_head->lowestRecPPEM, 
		lpTable_head->fontDirectionHint, 
		lpTable_head->indexToLocFormat, 
		lpTable_head->glyphDataFormat);

	return true;
}
