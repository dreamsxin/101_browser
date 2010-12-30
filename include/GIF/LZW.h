#ifndef _LZW_h
#define _LZW_h

#include "MiniStdlib/cstdint.h"
 // for size_t
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _LZW_Tree_Node
{
	struct _LZW_Tree_Node *pPrev;
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
void initLZW_Tree(LZW_Tree *out_pLZW_Tree, size_t in_tableSize);

/*!
 * You should not allocate this on a stack. Use the heap or
 * a global variable instead.
 */
typedef struct
{
	size_t stackSize;
	LZW_Tree_Node* pNodes[4097];
} LZW_Stack;

void initLZW_Stack(LZW_Stack *out_pLZW_Stack);

#ifdef __cplusplus
}
#endif

#endif
