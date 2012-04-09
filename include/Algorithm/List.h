/*
* Copyright 2012 Wolfgang Keller
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

#ifndef _Algorithm_List_h
#define _Algorithm_List_h

#include "MiniStdlib/cstddef.h" // for size_t
#include "MiniStdlib/declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
* Note: CDS is an abbreviation of Const Data Size
*/

typedef struct _CDSListNode
{
	struct _CDSListNode *pNext;
	char datum[0];
} CDSListNode;

typedef struct
{
	size_t datumSize;
	CDSListNode *pFirst;
} CDSList;

typedef struct
{
	void *pDatum;
	CDSList list;
} CDSListAppendData;

/*!
* Appends the datum in at the beginning of the list. NULL is returned if no 
* data could be allocated.
*
* If in_data.pDatum == NULL the original list is returned.
* 
* If allocation fails the returned list will have datumSize of 0.
*/
DLLEXPORT CDSList cdslist_append(CDSListAppendData in_data);

/*!
* Returns head element and tail of the list.
*/
DLLEXPORT CDSListAppendData cdslist_coappend(CDSList in_list);

/*!
* Preconditions:
* in_datumSize > 0
*/
DLLEXPORT CDSList cdslist_empty(size_t in_datumSize);

/*!
* Preconditions:
* in_list.datumSize > 0
*/
DLLEXPORT size_t cdslist_coempty(CDSList in_list);

#ifdef __cplusplus
}
#endif

#endif
