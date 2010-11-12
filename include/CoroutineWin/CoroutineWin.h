#ifndef _CoroutineWin_h
#define _CoroutineWin_h

#include <Windows.h>

typedef LPVOID CoroutineDescriptor;

CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (__stdcall * in_pFiberFunc)(void*), void* in_pParam);
void switchToCoroutine(CoroutineDescriptor in_coroutine);
CoroutineDescriptor convertThreadToCoroutine();
void deleteCoroutine(CoroutineDescriptor in_coroutine);

#endif
