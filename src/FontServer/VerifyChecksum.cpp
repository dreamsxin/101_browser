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

#include "FontServer/FontServer.h"
#include "BasicDataStructures/Endianess.h"

bool verifyCheckSum(FILE* fontFile, TableRecord* in_pTableRecord)
{
	fpos_t posBackup;
	
	if (fgetpos(fontFile, &posBackup) != 0)
		return false;

	bool headAdjustment = (in_pTableRecord->tag.uint == CHAR4_TO_UINT_LIL_ENDIAN('h', 'e', 'a', 'd'));

	unsigned int size = ((in_pTableRecord->length+3) & ~3)/4;
	fseek(fontFile, in_pTableRecord->offset, SEEK_SET);
	unsigned int sum=0;

	for (size_t i=0; i<size; i++)
	{
		if (i==2 && headAdjustment)
		{
			if (fseek(fontFile, 4, SEEK_CUR) != 0)
			{
				return false;
			}
		}
		else
		{
			unsigned int currentDword;

			/*
			* This means the file was probably not large enough for the given size.
			* So we have an invalid checksum (this error is recoverable - that is why
			* we return false and don't call exit)
			*/
			if (fread(&currentDword, sizeof(currentDword), 1, fontFile) != 1)
			{
				return false;
			}

			sum+=MTAx::Endianess::convertEndianess(currentDword);
		}
	}

	if (fsetpos(fontFile, &posBackup) != 0)
		return false;

	return sum == in_pTableRecord->checkSum;
}