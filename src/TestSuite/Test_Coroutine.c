#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Coroutine/Coroutine.h"

typedef struct
{
	CoroutineDescriptor currentDescriptor, func1Descriptor, func2Descriptor;
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

		switchToCoroutine(&pData->func2Descriptor);
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
			switchToCoroutine(&pData->currentDescriptor);
		else
			switchToCoroutine(&pData->func1Descriptor);
	}
}

void test_Coroutine()
{
	volatile unsigned int buffer[16];
	unsigned int index;
	PassedData passedData;
	bool bla;

	for (index = 0; index < 16; index++)
	{
		buffer[index] = 0xDEADBEAF;
	}

	passedData.currentDescriptor = convertThreadToCoroutine();

	test(passedData.currentDescriptor != NULL);

	passedData.func1Descriptor = createCoroutine(4096, &func1, &passedData);
	test(passedData.func1Descriptor != NULL);
	passedData.func2Descriptor = createCoroutine(4096, &func2, &passedData);
	test(passedData.func2Descriptor != NULL);

	passedData.pBuffer = buffer;
	passedData.currentIndex = 0;

	switchToCoroutine(&passedData.func1Descriptor);

	deleteCoroutine(&passedData.func1Descriptor);
	deleteCoroutine(&passedData.func2Descriptor);

	for (index = 0; index < 16; index++)
	{
		test(buffer[index] == index);
	}

	test(convertCoroutineToThread());
}
