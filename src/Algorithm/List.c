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

#include "Algorithm/List.h"
#include "MiniStdlib/cassert.h"
#include "MiniStdlib/MTAx_cstdlib.h" // for malloc
#include "MiniStdlib/cstring.h"

CDSList cdslist_append(CDSListAppendData in_data)
{
	CDSList out_list;

	assert(in_data.pDatum != NULL);
	assert(in_data.list.datumSize != 0);

	out_list.datumSize = in_data.list.datumSize;
	out_list.pFirst = (CDSListNode*) malloc(offsetof(CDSListNode, datum) + 
		in_data.list.datumSize);

	if (!out_list.pFirst)
	{
		out_list.datumSize = 0;
		return out_list;
	}

	memcpy(out_list.pFirst->datum, in_data.pDatum, in_data.list.datumSize);

	return out_list;
}

CDSListAppendData cdslist_coappend(CDSList in_list)
{
	CDSListAppendData out_appendData;

	out_appendData.list.datumSize = in_list.datumSize;

	if (in_list.pFirst)
	{
		out_appendData.list.pFirst = in_list.pFirst->pNext;
		out_appendData.pDatum = in_list.pFirst->datum;
	}
	else
	{
		out_appendData.list.pFirst = NULL;
		out_appendData.pDatum = NULL;
	}

	return out_appendData;
}

CDSList cdslist_empty(size_t in_datumSize)
{
	CDSList out_list;
	
	assert(in_datumSize > 0);

	out_list.datumSize = in_datumSize;
	out_list.pFirst = NULL;

	return out_list;
}

size_t cdslist_coempty(CDSList in_list)
{
	assert(in_list.datumSize > 0);

	return in_list.datumSize;
}
