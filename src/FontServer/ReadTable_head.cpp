#include <cassert>
#include "FontServer/FontServer.h"
#include "FontServer/FontServerUtil.h"

bool readTable_head(FILE* fontFile, TrueTypeFont* in_trueTypeFont)
{
	TableRecord* pTableRecord = getTableRecordPointer(fontFile, 
		&in_trueTypeFont->tableDirectory, 
		CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));
	
	if (pTableRecord == NULL)
		return false;

	if (fseek(fontFile, pTableRecord->offset, SEEK_SET) != 0)
		return false;

	Table_head lTable_head;

	if (pTableRecord->length != sizeof(lTable_head))
		return false;

	if (fread(&lTable_head, sizeof(lTable_head), 1, fontFile) != 1)
	{
		return false;
	}

	switchEndianess(&lTable_head.tableVersionNumber.beforeComma);
	switchEndianess(&lTable_head.tableVersionNumber. afterComma);
	switchEndianess(&lTable_head.fontRevision      .beforeComma);
	switchEndianess(&lTable_head.fontRevision      . afterComma);
	switchEndianess(&lTable_head.checkSumAdjustment            );
	switchEndianess(&lTable_head.magicNumber                   );
	switchEndianess(&lTable_head.flags                         );
	switchEndianess(&lTable_head.unitsPerEm                    );
	switchEndianess(&lTable_head.created                       );
	switchEndianess(&lTable_head.modified                      );
	switchEndianess(&lTable_head.xMin                          );
	switchEndianess(&lTable_head.yMin                          );
	switchEndianess(&lTable_head.xMax                          );
	switchEndianess(&lTable_head.xMax                          );
	switchEndianess(&lTable_head.macStyle                      );
	switchEndianess(&lTable_head.lowestRecPPEM                 );
	switchEndianess(&lTable_head.fontDirectionHint             );
	switchEndianess(&lTable_head.indexToLocFormat              );
	switchEndianess(&lTable_head.glyphDataFormat               );

	/*
	 * According to http://www.microsoft.com/typography/otspec/head.htm
	 * this field must have the value 0x5F0F3CF5
	 */
	if (lTable_head.magicNumber != 0x5F0F3CF5)
		return false;

	/*
	 * According to http://www.microsoft.com/typography/otspec/head.htm
	 * this field must be from 16 to 16384
	 */
	if (lTable_head.unitsPerEm < 16 || lTable_head.unitsPerEm > 16384)
		return false;

	if (lTable_head.glyphDataFormat != 0)
		return false;

	if (lTable_head.indexToLocFormat != 0 && lTable_head.indexToLocFormat != 1)
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
		lTable_head.tableVersionNumber.beforeComma, 
		lTable_head.tableVersionNumber. afterComma, 
		lTable_head.fontRevision      .beforeComma, 
		lTable_head.fontRevision      . afterComma, 
		lTable_head.checkSumAdjustment, 
		lTable_head.magicNumber, 
		lTable_head.flags, 
		lTable_head.unitsPerEm, 
#ifdef _WIN32
		lTable_head.created, 
		lTable_head.modified, 
#endif
		lTable_head.xMin, 
		lTable_head.yMin, 
		lTable_head.xMax, 
		lTable_head.xMax, 
		lTable_head.macStyle, 
		lTable_head.lowestRecPPEM, 
		lTable_head.fontDirectionHint, 
		lTable_head.indexToLocFormat, 
		lTable_head.glyphDataFormat);

	return true;
}