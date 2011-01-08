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
#include "FontServer/FontServerUtil.h"
// for memcmp
#include <cstring>

bool readOffsetTable(FILE* fontFile, OffsetTable* in_pOffsetTable)
{
	if (fread(in_pOffsetTable, sizeof(OffsetTable), 1, fontFile)!=1)
		return false;

	MTAx::Endianess::switchEndianess(&in_pOffsetTable->numTables);
	MTAx::Endianess::switchEndianess(&in_pOffsetTable->searchRange);
	MTAx::Endianess::switchEndianess(&in_pOffsetTable->entrySelector);
	MTAx::Endianess::switchEndianess(&in_pOffsetTable->rangeShift);

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

