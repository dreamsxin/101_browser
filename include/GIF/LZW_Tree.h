#ifndef _LZW_Tree_h
#define _LZW_Tree_h

#include "MiniStdlib/cstdint.h"

typedef struct _LZW_Tree_Node
{
	struct _LZW_Tree_Node *pNext;
	uint8_t code;
}  LZW_Tree_Node;

/*!
 * You should not allocate this on a stack. Use the heap or
 * a global variable instead.
 */
typedef struct
{
	size_t usedNodesCount;
	LZW_Tree_Node nodes[4097];
} LZW_Tree;

void initLZW_Tree(LZW_Tree *out_pLZW_Tree);

#endif
