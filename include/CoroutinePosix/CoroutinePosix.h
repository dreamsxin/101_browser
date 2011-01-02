#ifndef _MTAx_Coroutine_CoroutineWin_h
#define _MTAx_Coroutine_CoroutineWin_h

#include <ucontext.h>
#include <signal.h> // for SIGSTKSZ

#ifdef __cplusplus
extern "C" {
#endif

const size_t cCoroutineDefaultStackSize = SIGSTKSZ;

typedef ucontext_t CoroutineDescriptor;

CoroutineDescriptor createCoroutine(size_t in_stackSize,  void (*in_pFiberFunc)(void*), void* in_pParam);

void switchToCoroutine(CoroutineDescriptor *in_pCoroutine);

CoroutineDescriptor convertThreadToCoroutine();

/*!
 * Return value:
 * true - sucess
 * false - failure
 */
bool convertCoroutineToThread();

// getCurrentCoroutine is only used internally
CoroutineDescriptor getCurrentCoroutine();

void deleteCoroutine(CoroutineDescriptor *in_pCoroutine);

#ifdef __cplusplus
}
#endif

