#include "CoroutineWin/CoroutinePosix.h"
#include <stdlib.h>

CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (*in_pFiberFunc)(void*), void* in_pParam)
{
	CoroutineDescriptor out_descriptor;

	// Could be changed in future - only for now
	out_descriptor.uc_link = NULL;
	out_descriptor.uc_stack.ss_sp = malloc(in_stackSize);
	// TODO test for NULL
	out_descriptor.uc_stack.ss_size = sizeof(iterator_stack);

	getcontext(&out_descriptor);
	// TODO Fix portability issue in the following line
	makecontext(&out_descriptor, in_pFiberFunc, 1, in_pParam);

	return out_descriptor;
}



