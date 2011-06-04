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
#include "IO/BidirectionalStream.h"
#include "MiniStdlib/minmax.h"
#include "MiniStdlib/cstdbool.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct
{
	volatile size_t readCount;
	volatile bool endOfFunctionReached;
} EvilTestdata;

typedef bool (*BidirectionalStreamInitFunction)(void *out_pStreamState,
	ByteStreamInterface *out_pByteStreamInterface,
	bool in_startThisCoroutineAsWriter,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(ByteStreamReference in_byteStreamReference, void *in_pUserData),
	void *in_pUserData);



void evilReadFun(ByteStreamReference in_byteStreamReference, void *in_pUserdata)
{
	EvilTestdata *pTestdata = (EvilTestdata *) in_pUserdata;
	uint8_t buffer;

	test(in_byteStreamReference.mByteStreamInterface.mpfRead != NULL);

	if (NULL == in_byteStreamReference.mByteStreamInterface.mpfRead)
		return;

	while ((*in_byteStreamReference.mByteStreamInterface.mpfRead)
		(in_byteStreamReference.mpByteStreamState, &buffer, 1))
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
	ByteStreamInterface pipeStreamInterface;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	EvilTestdata testdata;
	bool result;

	{
		testdata.endOfFunctionReached = false;
		testdata.readCount = 0;

		result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface,
			true, &thisCoroutine, &otherCoroutine, 
			&evilReadFun, &testdata);

		test(result);

		if (!result)
			return;

		// we write nothing

		(*pipeStreamInterface.mpfWrite)(&pipeStreamState, NULL, 0);

		test(testdata.endOfFunctionReached);
		test(0 == testdata.readCount);

		pipeStreamStateDestroy(&pipeStreamState);
	}

	{
		uint8_t data[2];
		
		testdata.endOfFunctionReached = false;
		testdata.readCount = 0;

		result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface, 
			true, &thisCoroutine, &otherCoroutine, 
			&evilReadFun, &testdata);

		test(result);

		if (!result)
			return;

		(*pipeStreamInterface.mpfWrite)(&pipeStreamState, data, sizeof(data));

		(*pipeStreamInterface.mpfWrite)(&pipeStreamState, NULL, 0);

		test(testdata.endOfFunctionReached);
		test(2 == testdata.readCount);

		pipeStreamStateDestroy(&pipeStreamState);
	}
}



void readFun(ByteStreamReference in_byteStreamReference, unsigned int in_bytesToRead)
{
	uint8_t data[32];
	size_t readCount;
	unsigned int idx;

	test(in_byteStreamReference.mByteStreamInterface.mpfRead != NULL);

	if (NULL == in_byteStreamReference.mByteStreamInterface.mpfRead)
		return;

	memset(data, 0xFF, 32);

	printf("Reading %u bytes\n", in_bytesToRead);
	
	readCount = (*in_byteStreamReference.mByteStreamInterface.mpfRead)
		(in_byteStreamReference.mpByteStreamState, data, in_bytesToRead);

	test(readCount == MIN(in_bytesToRead, 4));

	for (idx = 0; idx < readCount; idx++)
	{
		test(idx == data[idx]);
	}
}

void writeFun(ByteStreamReference in_byteStreamReference, unsigned int in_bytesToRead)
{
	uint8_t data[3];

	test(in_byteStreamReference.mByteStreamInterface.mpfWrite != NULL);

	if (NULL == in_byteStreamReference.mByteStreamInterface.mpfWrite)
		return;

	data[0] = 0;

	printf("Writing 1 byte\n");
	test((*in_byteStreamReference.mByteStreamInterface.mpfWrite)
		(in_byteStreamReference.mpByteStreamState, data, 1) == 1);

	data[0] = 1;
	data[1] = 2;
	data[2] = 3;

	printf("Writing 3 bytes\n");
	test((*in_byteStreamReference.mByteStreamInterface.mpfWrite)
		(in_byteStreamReference.mpByteStreamState, data, 3) == MIN(3, in_bytesToRead-1));
}

void readerCoroutineFun(ByteStreamReference in_byteStreamReference, void *in_pUserdata)
{
	unsigned int *bytesToRead = (unsigned int *) in_pUserdata;
	readFun(in_byteStreamReference, *bytesToRead);
}

void writerCoroutineFun(ByteStreamReference in_byteStreamReference, void *in_pUserdata)
{
	unsigned int *bytesToRead = (unsigned int *) in_pUserdata;
	writeFun(in_byteStreamReference, *bytesToRead);
}

void pipeStreamOtherTests()
{
	PipeStreamState pipeStreamState;
	ByteStreamInterface pipeStreamInterface;
	ByteStreamReference pipeStreamReference;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	unsigned int bytesToReadArray[] = { 2, 32 };
	size_t idx;

	// Test 1, 2: the current coroutine is the writer and 2 or 32 bytes are read
	for (idx = 0; idx < 2; idx++)
	{
		bool result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface, 
			true, &thisCoroutine, &otherCoroutine, 
			&readerCoroutineFun, bytesToReadArray+idx);

		test(result);

		if (!result)
			return;

		pipeStreamReference.mByteStreamInterface = pipeStreamInterface;
		pipeStreamReference.mpByteStreamState = &pipeStreamState;

		writeFun(pipeStreamReference, bytesToReadArray[idx]);
		test((*pipeStreamInterface.mpfWrite)(&pipeStreamState, NULL, 0) == 0);

		pipeStreamStateDestroy(&pipeStreamState);
	}

	// Test 3, 4: the current coroutine is the reader and 2 or 32 bytes are read
	for (idx = 0; idx < 2; idx++)
	{
		bool result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface, 
			false, &thisCoroutine, &otherCoroutine, 
			&writerCoroutineFun, bytesToReadArray+idx);

		test(result);

		if (!result)
			return;

		pipeStreamReference.mByteStreamInterface = pipeStreamInterface;
		pipeStreamReference.mpByteStreamState = &pipeStreamState;

		readFun(pipeStreamReference, bytesToReadArray[idx]);
		test((*pipeStreamInterface.mpfRead)(&pipeStreamState, NULL, 0) == 0);

		pipeStreamStateDestroy(&pipeStreamState);
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

	printf("Testing IO - BidirectionalStream\n");
	// TODO
}
