#include "CoroutineWin/CoroutineWin.h"

CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (__stdcall * in_pFiberFunc)(void*), void* in_pParam)
{
	return CreateFiber(in_stackSize, in_pFiberFunc, in_pParam);
}

void switchToCoroutine(CoroutineDescriptor in_coroutine)
{
	SwitchToFiber(in_coroutine);
}

CoroutineDescriptor convertThreadToCoroutine()
{
	// we pass no data since this is not necessary
	return ConvertThreadToFiber(NULL);
}

void deleteCoroutine(CoroutineDescriptor in_coroutine)
{
	DeleteFiber(in_coroutine);
}
