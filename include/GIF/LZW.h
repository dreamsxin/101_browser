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

#ifndef _LZW_h
#define _LZW_h

#include "MiniStdlib/cstdint.h"
 // for size_t
#ifdef __cplusplus
#include <climits>
#else
#include <limits.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
* Q: What do we need this for?
* A: Because some LZW Tree Nodes have no predecessor and we store indices 
*    instead of pointers, we need some value that fills the role of the NULL
*    pointer. This is what this sentinel is for.
*/
#define PREVIOUS_LZW_TREE_NODE_INDEX_SENTINEL _UI16_MAX

typedef struct
{
	uint16_t previousLzwTreeNodeIndex;
	uint8_t firstCode;
	uint8_t lastCode;
}  LZW_Tree_Node;

/*!
 * You should not allocate this on a stack. Use the heap or
 * a global variable instead.
 */
typedef struct
{
	LZW_Tree_Node nodes[4097];
} LZW_Tree;

/*!
 * Initializes the terminal nodes (color table entries) in the LZW tree
 */
void initLZW_Tree(LZW_Tree *out_pLZW_Tree, uint16_t in_tableSize);

/*!
 * You should not allocate this on a stack. Use the heap or
 * a global variable instead.
 */
typedef struct
{
	uint16_t stackSize;
	uint16_t lzwTreeIndices[4097];
} LZW_Stack;

void initLZW_Stack(LZW_Stack *out_pLZW_Stack);

#ifdef __cplusplus
}
#endif

#endif
