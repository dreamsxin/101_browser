#include "Coroutine/Coroutine.h"
#include "MiniStdlib/safe_free.h"
#include <stdlib.h>


bool createCoroutine(size_t in_stackSize, 
	void (*in_pFiberFunc)(void*), 
	void* in_pParam, 
	CoroutineDescriptor *out_pCoroutineDescriptor)
{
	CoroutineDescriptor out_descriptor;

	// Could be changed in future - only for now
	out_descriptor.uc_link = NULL;
	out_descriptor.uc_stack.ss_sp = malloc(in_stackSize);
	if (NULL == out_descriptor.uc_stack.ss_sp)
		return false;
	out_descriptor.uc_stack.ss_size = in_stackSize;

	getcontext(&out_descriptor);
	// TODO Fix 64 bit portability issue in the following line
	makecontext(&out_descriptor, in_pFiberFunc, 1, in_pParam);

	return out_descriptor;
}

void switchToCoroutine(volatile CoroutineDescriptor * in_pCurrentCoroutine, volatile CoroutineDescriptor *in_pNextCoroutine)
{
	swapcontext(in_pCurrentCoroutine, in_pNextCoroutine);
}

CoroutineDescriptor convertThreadToCoroutine()
{
	CoroutineDescriptor out_coroutineDescriptor;
	// TODO Check error code
	getcontext(&out_coroutineDescriptor);
	return out_coroutineDescriptor;
}

bool convertCoroutineToThread()
{
	return true;
}

void deleteCoroutine(volatile CoroutineDescriptor *in_pCoroutine)
{
	safe_free(&in_pCoroutine->uc_stack.ss_sp);
}
