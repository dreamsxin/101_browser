#ifndef _CoroutineWin_h
#define _CoroutineWin_h

#include <Windows.h>
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef LPVOID CoroutineDescriptor;

#ifdef _WIN32
__declspec(dllexport)
#endif
CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (__stdcall * in_pFiberFunc)(void*), void* in_pParam);
#ifdef _WIN32
__declspec(dllexport)
#endif
void switchToCoroutine(CoroutineDescriptor *in_pCoroutine);

/*!
 * Return value:
 * true - sucess
 * false - failure
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
CoroutineDescriptor convertThreadToCoroutine();
#ifdef _WIN32
__declspec(dllexport)
#endif
bool convertCoroutineToThread();
// this is only used internally
CoroutineDescriptor getCurrentCoroutine();
#ifdef _WIN32
__declspec(dllexport)
#endif
void deleteCoroutine(CoroutineDescriptor *in_pCoroutine);

#ifdef __cplusplus
}
#endif

#endif
