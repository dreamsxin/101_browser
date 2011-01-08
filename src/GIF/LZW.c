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

#include "GIF/LZW.h"
#include <assert.h>
#include <stdlib.h>

void initLZW_Tree(LZW_Tree *out_pLZW_Tree, size_t in_tableSize)
{
	size_t i;

	for (i = 0; i < in_tableSize; i++)
	{
		out_pLZW_Tree->nodes[i].pPrev = NULL;
		out_pLZW_Tree->nodes[i].firstCode = (uint8_t) i;
		out_pLZW_Tree->nodes[i].lastCode = (uint8_t) i;
	}
}

void initLZW_Stack(LZW_Stack *out_pLZW_Stack)
{
	out_pLZW_Stack->stackSize = 0;
}
