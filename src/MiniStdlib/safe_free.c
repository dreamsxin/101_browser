#include <stdlib.h>
#include "MiniStdlib/safe_free.h"

void safe_free(void* in_ppT)
{
	free(*(void**) in_ppT);
	*(void**) in_ppT = NULL;
}
