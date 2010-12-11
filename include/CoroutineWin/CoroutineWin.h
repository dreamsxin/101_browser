#ifndef _CoroutineWin_h
#define _CoroutineWin_h

#include <Windows.h>

typedef LPVOID CoroutineDescriptor;

#ifdef _WIN32
__declspec(dllexport)
#endif
CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (__stdcall * in_pFiberFunc)(void*), void* in_pParam);
#ifdef _WIN32
__declspec(dllexport)
#endif
void switchToCoroutine(CoroutineDescriptor *in_pCoroutine);
#ifdef _WIN32
__declspec(dllexport)
#endif
CoroutineDescriptor convertThreadToCoroutine();
// this is only used internally
CoroutineDescriptor getCurrentCoroutine();
#ifdef _WIN32
__declspec(dllexport)
#endif
void deleteCoroutine(CoroutineDescriptor in_coroutine);

#endif
