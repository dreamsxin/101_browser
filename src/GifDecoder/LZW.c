#include "GIF/LZW.h"
#include <assert.h>
#include <stdlib.h>

void initLZW_Tree(LZW_Tree *out_pLZW_Tree, size_t in_tableSize)
{
	size_t i;

	for (i = 0; i < in_tableSize; i++)
	{
		out_pLZW_Tree->nodes[i].firstCode = (uint8_t) i;
		out_pLZW_Tree->nodes[i].pPrev = NULL;
	}

}

void initLZW_Stack(LZW_Stack *out_pLZW_Stack)
{
	out_pLZW_Stack->stackSize = 0;
}

