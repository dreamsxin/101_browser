#ifndef _MTAx_Coroutine_CoroutineWin_h
#define _MTAx_Coroutine_CoroutineWin_h

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef LPVOID CoroutineDescriptor;

__declspec(dllexport) bool createCoroutine(
	size_t in_stackSize,  
	void (__stdcall * in_pFiberFunc)(void*), 
	void* in_pParam, 
	volatile CoroutineDescriptor *out_pCoroutineDescriptor);

__declspec(dllexport) void switchToCoroutine(
	volatile CoroutineDescriptor * in_pCurrentCoroutine, 
	volatile CoroutineDescriptor *in_pNextCoroutine);

/*!
 * Return value:
 * true - sucess
 * false - failure
 */
__declspec(dllexport) CoroutineDescriptor convertThreadToCoroutine();

__declspec(dllexport) bool convertCoroutineToThread();

// getCurrentCoroutine is only used internally
CoroutineDescriptor getCurrentCoroutine();

__declspec(dllexport) void deleteCoroutine(volatile CoroutineDescriptor *in_pCoroutine);

#ifdef __cplusplus
}
#endif

#endif
