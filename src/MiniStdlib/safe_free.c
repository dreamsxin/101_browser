#include <stdlib.h>
#include <assert.h>
#include "MiniStdlib/safe_free.h"

void safe_free(void* in_ppT)
{
	assert(in_ppT != NULL);

	free(*(void**) in_ppT);
	*(void**) in_ppT = NULL;
}
