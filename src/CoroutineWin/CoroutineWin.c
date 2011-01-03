#include "Coroutine/Coroutine.h"
#include <assert.h>

bool createCoroutine(
	size_t in_stackSize,  
	void (__stdcall * in_pFiberFunc)(void*), 
	void* in_pParam, 
	volatile CoroutineDescriptor *out_pCoroutineDescriptor)
{
	CoroutineDescriptor lCoroutineDescriptor = 
		CreateFiber(in_stackSize, in_pFiberFunc, in_pParam);
	if (lCoroutineDescriptor != NULL)
	{
		*out_pCoroutineDescriptor = lCoroutineDescriptor;
		return true;
	}
	else
		return false;
}

void switchToCoroutine(
	volatile CoroutineDescriptor * in_pCurrentCoroutine, 
	volatile CoroutineDescriptor *in_pNextCoroutine)
{
	if (in_pNextCoroutine != NULL)
	{
		// Under Windows this is no problem - but it will cause problems under Linux
		assert(getCurrentCoroutine() == *in_pCurrentCoroutine);

		SwitchToFiber(*in_pNextCoroutine);
	}
}

CoroutineDescriptor getCurrentCoroutine()
{
	if (IsThreadAFiber())
		return GetCurrentFiber();
	else
		return NULL;
}

bool convertThreadToCoroutine(
	CoroutineDescriptor *out_pCoroutineDescriptor)
{
	CoroutineDescriptor lCoroutineDescriptor = getCurrentCoroutine();

	if (NULL == lCoroutineDescriptor)
	{
		/*
		 * According to
		 * http://msdn.microsoft.com/en-us/library/ms682115(VS.85).aspx
		 * "If the function succeeds, the return value is the address of the fiber.
		 * 
		 * If the function fails, the return value is NULL. 
		 * To get extended error information, call GetLastError."
		 */
		lCoroutineDescriptor = ConvertThreadToFiber(NULL);
		
		if (NULL == lCoroutineDescriptor)
			return false;
		else
		{
			*out_pCoroutineDescriptor = lCoroutineDescriptor;
			return true;
		}
	}
	else
	{
		*out_pCoroutineDescriptor = lCoroutineDescriptor;
		return true;
	}
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

void deleteCoroutine(volatile CoroutineDescriptor *in_pCoroutine)
{
	DeleteFiber(*in_pCoroutine);
	*in_pCoroutine = NULL;
}
