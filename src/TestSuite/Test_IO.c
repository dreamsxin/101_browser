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
#include <string.h>

void readFun(PipeStreamState *in_pPipeStreamState, unsigned int in_bytesToRead)
{
	uint8_t data[32];
	size_t readCount;
	unsigned int idx;

	memset(data, 0xFF, 32);

	wprintf(L"Reading %u bytes\n", in_bytesToRead);
	
	readCount = pipeStreamRead(in_pPipeStreamState, data, in_bytesToRead);

	test(readCount == MIN(in_bytesToRead, 4));

	for (idx = 0; idx < readCount; idx++)
	{
		test(idx == data[idx]);
	}
}

void writeFun(PipeStreamState *in_pPipeStreamState, unsigned int in_bytesToRead)
{
	uint8_t data[3];

	data[0] = 0;

	wprintf(L"Writing 1 byte\n");
	test(pipeStreamWrite(in_pPipeStreamState, data, 1) == 1);

	data[0] = 1;
	data[1] = 2;
	data[2] = 3;

	wprintf(L"Writing 3 bytes\n");
	test(pipeStreamWrite(in_pPipeStreamState, data, 3) == MIN(3, in_bytesToRead-1));
}

void readerCoroutineFun(PipeStreamState *in_pPipeStreamState, void *in_pUserdata)
{
	unsigned int *bytesToRead = (unsigned int *) in_pUserdata;
	readFun(in_pPipeStreamState, *bytesToRead);
}

void writerCoroutineFun(PipeStreamState *in_pPipeStreamState, void *in_pUserdata)
{
	unsigned int *bytesToRead = (unsigned int *) in_pUserdata;
	writeFun(in_pPipeStreamState, *bytesToRead);
}

void test_IO_PipeStream()
{
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	unsigned int bytesToRead;

	// Test 1: the current coroutine is the writer and 2 bytes are read
	bytesToRead = 2;
	{
		bool result = initPipeStreamState(&pipeStreamState, true, 
			&thisCoroutine, &otherCoroutine, 
			&readerCoroutineFun, &bytesToRead);

		test(result);

		if (!result)
			return;

		writeFun(&pipeStreamState, bytesToRead);
		test(pipeStreamWrite(&pipeStreamState, NULL, 0) == 0);

		deletePipeStreamState(&pipeStreamState);
	}

	// Test 2: the current coroutine is the writer and 32 bytes are read
	bytesToRead = 32;
	{
		bool result = initPipeStreamState(&pipeStreamState, true, 
			&thisCoroutine, &otherCoroutine, 
			&readerCoroutineFun, &bytesToRead);

		test(result);

		if (!result)
			return;

		writeFun(&pipeStreamState, bytesToRead);
		test(pipeStreamWrite(&pipeStreamState, NULL, 0) == 0);

		deletePipeStreamState(&pipeStreamState);
	}

	// Test 3: the current coroutine is the reader and 2 bytes are read
	bytesToRead = 2;
	{
		bool result = initPipeStreamState(&pipeStreamState, false, 
			&thisCoroutine, &otherCoroutine, 
			&writerCoroutineFun, &bytesToRead);

		test(result);

		if (!result)
			return;

		readFun(&pipeStreamState, bytesToRead);
		test(pipeStreamRead(&pipeStreamState, NULL, 0) == 0);

		deletePipeStreamState(&pipeStreamState);
	}

	// Test 4: the current coroutine is the reader and 32 bytes are read
	bytesToRead = 32;
	{
		bool result = initPipeStreamState(&pipeStreamState, false, 
			&thisCoroutine, &otherCoroutine, 
			&writerCoroutineFun, &bytesToRead);

		test(result);

		if (!result)
			return;

		readFun(&pipeStreamState, bytesToRead);
		test(pipeStreamRead(&pipeStreamState, NULL, 0) == 0);

		deletePipeStreamState(&pipeStreamState);
	}
}

void test_IO()
{
	wprintf(L"Testing IO - PipeStream\n");
	test_IO_PipeStream();
}
