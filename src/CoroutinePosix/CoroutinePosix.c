#include "Coroutine/Coroutine.h"
#include "MiniStdlib/safe_free.h"
#include <stdlib.h>


bool createCoroutine(size_t in_stackSize, 
	void (*in_pFiberFunc)(void*), 
	void* in_pParam, 
	CoroutineDescriptor *out_pCoroutineDescriptor)
{
	CoroutineDescriptor lCoroutineDescriptor;

	// Could be changed in future - only for now
	lCoroutineDescriptor.uc_link = NULL;
	lCoroutineDescriptor.uc_stack.ss_sp = malloc(in_stackSize);
	if (NULL == lCoroutineDescriptor.uc_stack.ss_sp)
		return false;
	lCoroutineDescriptor.uc_stack.ss_size = in_stackSize;

	getcontext(&lCoroutineDescriptor);
	// TODO Fix 64 bit portability issue in the following line
	makecontext(&lCoroutineDescriptor, in_pFiberFunc, 1, in_pParam);

	*out_pCoroutineDescriptor = lCoroutineDescriptor;
	return true;
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
