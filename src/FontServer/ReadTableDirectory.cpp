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

bool readTableRecord(FILE* fontFile, TableRecord* in_pTableRecord)
{
	if (fread(in_pTableRecord, sizeof(TableRecord), 1, fontFile) != 1)
		return false;

	MTAx::Endianess::switchEndianess(&in_pTableRecord->checkSum);
	MTAx::Endianess::switchEndianess(&in_pTableRecord->offset  );
	MTAx::Endianess::switchEndianess(&in_pTableRecord->length  );

	return true;
}
