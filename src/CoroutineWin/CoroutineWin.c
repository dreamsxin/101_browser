#include "Coroutine/Coroutine.h"

CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (__stdcall * in_pFiberFunc)(void*), void* in_pParam)
{
	return CreateFiber(in_stackSize, in_pFiberFunc, in_pParam);
}

void switchToCoroutine(CoroutineDescriptor *in_pCoroutine)
{
	SwitchToFiber(*in_pCoroutine);
}

CoroutineDescriptor getCurrentCoroutine()
{
	if (IsThreadAFiber())
		return GetCurrentFiber();
	else
		return NULL;
}

CoroutineDescriptor convertThreadToCoroutine()
{
	CoroutineDescriptor lCoroutineDescriptor = getCurrentCoroutine();

	if (!lCoroutineDescriptor)
		// we pass no data since this is not necessary
		return ConvertThreadToFiber(NULL);
	else
		return lCoroutineDescriptor;
}

bool convertCoroutineToThread()
{
	CoroutineDescriptor lCoroutineDescriptor = getCurrentCoroutine();

	if (lCoroutineDescriptor)
		/*!
		 * According to
		 * http://msdn.microsoft.com/en-us/library/ms682112(VS.85).aspx
		 * "If the function succeeds, the return value is nonzero."
		 * 
		 * We want to be sure that it is either 0 or 1.
		 */
		return ConvertFiberToThread() != 0;
	else
		return true;
}

void deleteCoroutine(CoroutineDescriptor *in_pCoroutine)
{
	DeleteFiber(*in_pCoroutine);
	*in_pCoroutine = NULL;
}

