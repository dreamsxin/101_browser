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
#include "FontServer/FontServerUtil.h"

TableRecord* getTableRecordPointer(FILE* fontFile, const ArrayBlock<TableRecord>* in_pTableDirectory, UINT in_tag)
{
	for (size_t currentTableRecordIndex = 0; 
		currentTableRecordIndex < in_pTableDirectory->count();
		currentTableRecordIndex++)
	{
		TableRecord* pCurrentTableRecord = in_pTableDirectory->data()+currentTableRecordIndex;

		if (pCurrentTableRecord->tag.uint == in_tag)
		{
			return pCurrentTableRecord;
		}
	}

	return NULL;
}
