/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Coroutine/Coroutine.h"
#include "MiniStdlib/safe_free.h"
#include <stdlib.h>

#define DEFAULT_COROUTINE_STACK_SIZE 0x10000

bool createCoroutine(size_t in_stackSize, 
	void (*in_pFiberFunc)(void*), 
	void* in_pParam, 
	volatile CoroutineDescriptor *out_pCoroutineDescriptor)
{
	CoroutineDescriptor lCoroutineDescriptor;

	// Could be changed in future - only for now
	lCoroutineDescriptor.uc_link = NULL;

	if (in_stackSize == 0)
		in_stackSize = DEFAULT_COROUTINE_STACK_SIZE;

	lCoroutineDescriptor.uc_stack.ss_sp = malloc(in_stackSize);
	if (NULL == lCoroutineDescriptor.uc_stack.ss_sp)
		return false;
	lCoroutineDescriptor.uc_stack.ss_size = in_stackSize;

	getcontext(&lCoroutineDescriptor);
	// TODO Fix 64 bit portability issue in the following line
	makecontext(&lCoroutineDescriptor, (void (*)(void)) in_pFiberFunc, 1, in_pParam);

	*out_pCoroutineDescriptor = lCoroutineDescriptor;
	return true;
}

bool switchToCoroutine(CoroutineDescriptor * in_pCurrentCoroutine, CoroutineDescriptor *in_pNextCoroutine)
{
	return swapcontext(in_pCurrentCoroutine, in_pNextCoroutine) != 0;
}

bool convertThreadToCoroutine(
	CoroutineDescriptor *out_pCoroutineDescriptor)
{
	/*
	 * According to
	 * http://pubs.opengroup.org/onlinepubs/007908799/xsh/getcontext.html
	 * "On successful completion, setcontext() does not return and getcontext() 
	 * returns 0. Otherwise, a value of -1 is returned."
	 */
	return getcontext(out_pCoroutineDescriptor) == 0;
}

bool convertCoroutineToThread()
{
	return true;
}

void deleteCoroutine(CoroutineDescriptor *in_pCoroutine)
{
	safe_free(&in_pCoroutine->uc_stack.ss_sp);
}
