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
#include "IO/PipeStream.h"
#include "MiniStdlib/minmax.h"
#include <stdio.h>
#include <string.h>

typedef struct
{
	volatile size_t readCount;
	volatile bool endOfFunctionReached;
} EvilTestdata;

void evilReadFun(void *in_pPipeStreamState, void *in_pUserdata)
{
	EvilTestdata *pTestdata = (EvilTestdata *) in_pUserdata;
	uint8_t buffer;

	while (pipeStreamRead(in_pPipeStreamState, &buffer, 1))
		pTestdata->readCount++;

	pTestdata->endOfFunctionReached = true;
}

void testPipeStreamEvil()
{
	/*
	* See documentation (PipeStream example 1) for explanation 
	* what this example does. 
	* The example in the documentation is simplified for simplicity
	* 
	*/
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	EvilTestdata testdata;
	bool result;

	{
		testdata.endOfFunctionReached = false;
		testdata.readCount = 0;

		result = pipeStreamInit(&pipeStreamState, true, 
			&thisCoroutine, &otherCoroutine, 
			&evilReadFun, &testdata);

		test(result);

		if (!result)
			return;

		// we write nothing

		pipeStreamWrite(&pipeStreamState, NULL, 0);

		test(testdata.endOfFunctionReached);
		test(0 == testdata.readCount);

		deletePipeStreamState(&pipeStreamState);
	}

	{
		uint8_t data[2];
		
		testdata.endOfFunctionReached = false;
		testdata.readCount = 0;

		result = pipeStreamInit(&pipeStreamState, true, 
			&thisCoroutine, &otherCoroutine, 
			&evilReadFun, &testdata);

		test(result);

		if (!result)
			return;

		pipeStreamWrite(&pipeStreamState, data, sizeof(data));

		pipeStreamWrite(&pipeStreamState, NULL, 0);

		test(testdata.endOfFunctionReached);
		test(2 == testdata.readCount);

		deletePipeStreamState(&pipeStreamState);
	}
}



void readFun(void *in_pPipeStreamState, unsigned int in_bytesToRead)
{
	uint8_t data[32];
	size_t readCount;
	unsigned int idx;

	memset(data, 0xFF, 32);

	printf("Reading %u bytes\n", in_bytesToRead);
	
	readCount = pipeStreamRead(in_pPipeStreamState, data, in_bytesToRead);

	test(readCount == MIN(in_bytesToRead, 4));

	for (idx = 0; idx < readCount; idx++)
	{
		test(idx == data[idx]);
	}
}

void writeFun(void *in_pPipeStreamState, unsigned int in_bytesToRead)
{
	uint8_t data[3];

	data[0] = 0;

	printf("Writing 1 byte\n");
	test(pipeStreamWrite(in_pPipeStreamState, data, 1) == 1);

	data[0] = 1;
	data[1] = 2;
	data[2] = 3;

	printf("Writing 3 bytes\n");
	test(pipeStreamWrite(in_pPipeStreamState, data, 3) == MIN(3, in_bytesToRead-1));
}

void readerCoroutineFun(void *in_pPipeStreamState, void *in_pUserdata)
{
	unsigned int *bytesToRead = (unsigned int *) in_pUserdata;
	readFun(in_pPipeStreamState, *bytesToRead);
}

void writerCoroutineFun(void *in_pPipeStreamState, void *in_pUserdata)
{
	unsigned int *bytesToRead = (unsigned int *) in_pUserdata;
	writeFun(in_pPipeStreamState, *bytesToRead);
}

void pipeStreamOtherTests()
{
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	unsigned int bytesToReadArray[] = { 2, 32 };
	size_t idx;

	// Test 1, 2: the current coroutine is the writer and 2 or 32 bytes are read
	for (idx = 0; idx < 2; idx++)
	{
		bool result = pipeStreamInit(&pipeStreamState, true, 
			&thisCoroutine, &otherCoroutine, 
			&readerCoroutineFun, bytesToReadArray+idx);

		test(result);

		if (!result)
			return;

		writeFun(&pipeStreamState, bytesToReadArray[idx]);
		test(pipeStreamWrite(&pipeStreamState, NULL, 0) == 0);

		deletePipeStreamState(&pipeStreamState);
	}

	// Test 3, 4: the current coroutine is the reader and 2 or 32 bytes are read
	for (idx = 0; idx < 2; idx++)
	{
		bool result = pipeStreamInit(&pipeStreamState, false, 
			&thisCoroutine, &otherCoroutine, 
			&writerCoroutineFun, bytesToReadArray+idx);

		test(result);

		if (!result)
			return;

		readFun(&pipeStreamState, bytesToReadArray[idx]);
		test(pipeStreamRead(&pipeStreamState, NULL, 0) == 0);

		deletePipeStreamState(&pipeStreamState);
	}
}

void test_IO_PipeStream()
{
	printf("Testing PipeStream behaviour in \"evil\" cases.\n");
	testPipeStreamEvil();

	printf("Doing other PipeStream tests.\n");
	pipeStreamOtherTests();
}

void test_IO()
{
	printf("Testing IO - PipeStream\n");
	test_IO_PipeStream();
}
