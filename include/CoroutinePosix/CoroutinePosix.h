#ifndef _MTAx_Coroutine_CoroutineWin_h
#define _MTAx_Coroutine_CoroutineWin_h

#include <ucontext.h>
#include <signal.h> // for SIGSTKSZ

#ifdef __cplusplus
extern "C" {
#endif

const size_t cCoroutineDefaultStackSize = SIGSTKSZ;

typedef ucontext_t CoroutineDescriptor;

bool createCoroutine(size_t in_stackSize, 
	void (*in_pFiberFunc)(void*), 
	void* in_pParam, 
	CoroutineDescriptor *out_pCoroutineDescriptor);

void switchToCoroutine(
	volatile CoroutineDescriptor * in_pCurrentCoroutine, 
	volatile CoroutineDescriptor *in_pNextCoroutine);

CoroutineDescriptor convertThreadToCoroutine();

/*!
 * Return value:
 * true - sucess
 * false - failure
 */
bool convertCoroutineToThread();

/*!
 * Warning: never call it on a coroutine that was not created by createCoroutine.
 * 
 * You have been warned.
 */
void deleteCoroutine(volatile CoroutineDescriptor *in_pCoroutine);

#ifdef __cplusplus
}
#endif

#endif

