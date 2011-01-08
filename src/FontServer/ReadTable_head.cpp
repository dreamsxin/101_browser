/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cassert>
#include "FontServer/FontServer.h"
#include "BasicDataStructures/Endianess.h"

bool readTable_head(FILE* fontFile, TrueTypeFont* in_trueTypeFont, Table_head* in_pTable_head)
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

	if (fread(in_pTable_head, sizeof(Table_head), 1, fontFile) != 1)
	{
		return false;
	}

	MTAx::Endianess::switchEndianess(&in_pTable_head->tableVersionNumber.beforeComma);
	MTAx::Endianess::switchEndianess(&in_pTable_head->tableVersionNumber. afterComma);
	MTAx::Endianess::switchEndianess(&in_pTable_head->fontRevision      .beforeComma);
	MTAx::Endianess::switchEndianess(&in_pTable_head->fontRevision      . afterComma);
	MTAx::Endianess::switchEndianess(&in_pTable_head->checkSumAdjustment            );
	MTAx::Endianess::switchEndianess(&in_pTable_head->magicNumber                   );
	MTAx::Endianess::switchEndianess(&in_pTable_head->flags                         );
	MTAx::Endianess::switchEndianess(&in_pTable_head->unitsPerEm                    );
	MTAx::Endianess::switchEndianess(&in_pTable_head->created                       );
	MTAx::Endianess::switchEndianess(&in_pTable_head->modified                      );
	MTAx::Endianess::switchEndianess(&in_pTable_head->xMin                          );
	MTAx::Endianess::switchEndianess(&in_pTable_head->yMin                          );
	MTAx::Endianess::switchEndianess(&in_pTable_head->xMax                          );
	MTAx::Endianess::switchEndianess(&in_pTable_head->xMax                          );
	MTAx::Endianess::switchEndianess(&in_pTable_head->macStyle                      );
	MTAx::Endianess::switchEndianess(&in_pTable_head->lowestRecPPEM                 );
	MTAx::Endianess::switchEndianess(&in_pTable_head->fontDirectionHint             );
	MTAx::Endianess::switchEndianess(&in_pTable_head->indexToLocFormat              );
	MTAx::Endianess::switchEndianess(&in_pTable_head->glyphDataFormat               );

	if (in_pTable_head->tableVersionNumber.beforeComma != 1 || 
		in_pTable_head->tableVersionNumber.afterComma  != 0)
		return false;

	/*
	 * According to http://www.microsoft.com/typography/otspec/head.htm
	 * this field must have the value 0x5F0F3CF5
	 */
	if (in_pTable_head->magicNumber != 0x5F0F3CF5)
		return false;

	/*
	 * According to http://www.microsoft.com/typography/otspec/head.htm
	 * this field must be from 16 to 16384
	 */
	if (in_pTable_head->unitsPerEm < 16 || in_pTable_head->unitsPerEm > 16384)
		return false;

	if (in_pTable_head->glyphDataFormat != 0)
		return false;

	if (in_pTable_head->indexToLocFormat != 0 && in_pTable_head->indexToLocFormat != 1)
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
		in_pTable_head->tableVersionNumber.beforeComma, 
		in_pTable_head->tableVersionNumber. afterComma, 
		in_pTable_head->fontRevision      .beforeComma, 
		in_pTable_head->fontRevision      . afterComma, 
		in_pTable_head->checkSumAdjustment, 
		in_pTable_head->magicNumber, 
		in_pTable_head->flags, 
		in_pTable_head->unitsPerEm, 
#ifdef _WIN32
		in_pTable_head->created, 
		in_pTable_head->modified, 
#endif
		in_pTable_head->xMin, 
		in_pTable_head->yMin, 
		in_pTable_head->xMax, 
		in_pTable_head->xMax, 
		in_pTable_head->macStyle, 
		in_pTable_head->lowestRecPPEM, 
		in_pTable_head->fontDirectionHint, 
		in_pTable_head->indexToLocFormat, 
		in_pTable_head->glyphDataFormat);

	return true;
}
