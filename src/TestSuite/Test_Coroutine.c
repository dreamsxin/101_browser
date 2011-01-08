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

#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Coroutine/Coroutine.h"

typedef struct
{
	CoroutineDescriptor *pCurrentDescriptor, *pFunc1Descriptor, *pFunc2Descriptor;
	volatile unsigned int *pBuffer;
	volatile unsigned int currentIndex;
} PassedData;

void 
#ifdef _WIN32
__stdcall
#endif	
func1(void *in_pData)
{
	volatile PassedData *pData = (volatile PassedData*) in_pData;

	while (1)
	{
		pData->pBuffer[pData->currentIndex] = pData->currentIndex;
		pData->currentIndex++;

		switchToCoroutine(pData->pFunc1Descriptor, pData->pFunc2Descriptor);
	}
}

void
#ifdef _WIN32
__stdcall
#endif
func2(void *in_pData)
{
	volatile PassedData *pData = (volatile PassedData*) in_pData;

	while (1)
	{
		pData->pBuffer[pData->currentIndex] = pData->currentIndex;
		pData->currentIndex++;

		if (pData->currentIndex == 16)
			switchToCoroutine(pData->pFunc2Descriptor, pData->pCurrentDescriptor);
		else
			switchToCoroutine(pData->pFunc2Descriptor, pData->pFunc1Descriptor);
	}
}

void test_Coroutine()
{
	volatile unsigned int buffer[16];
	unsigned int index;
	bool result;
	PassedData passedData;
	CoroutineDescriptor currentDescriptor, func1Descriptor, func2Descriptor;

	for (index = 0; index < 16; index++)
	{
		buffer[index] = 0xDEADBEAF;
	}

	result = convertThreadToCoroutine(&currentDescriptor);
#ifdef _WIN32
	test(currentDescriptor != NULL);
#endif
	passedData.pCurrentDescriptor = &currentDescriptor;

	result = createCoroutine(4096, &func1, &passedData, &func1Descriptor);
	test(result);
	passedData.pFunc1Descriptor = &func1Descriptor;

	result = createCoroutine(4096, &func2, &passedData, &func2Descriptor);
	test(result);
	passedData.pFunc2Descriptor = &func2Descriptor;

	passedData.pBuffer = buffer;
	passedData.currentIndex = 0;

	switchToCoroutine(passedData.pCurrentDescriptor, passedData.pFunc1Descriptor);

	deleteCoroutine(passedData.pFunc1Descriptor);
	deleteCoroutine(passedData.pFunc2Descriptor);

	for (index = 0; index < 16; index++)
	{
		test(buffer[index] == index);
	}

	test(convertCoroutineToThread());
}
