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

void readerCoroutineFun(PipeStreamState *in_pPipeStreamState, void *in_pUserdata)
{
	uint8_t data[2] = {0xFF, 0xFF};

	wprintf(L"Reading 2 bytes\n");
	pipeStreamRead(in_pPipeStreamState, data, 2);

	test(0 == data[0]);
	test(1 == data[1]);
}

void test_IO_PipeStream()
{
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;

	{
		bool result = initPipeStreamState(&pipeStreamState, true, 
			&thisCoroutine, &otherCoroutine, 
			&readerCoroutineFun, NULL);
		uint8_t data[3];

		test(result);

		if (!result)
			return;

		data[0] = 0;

		wprintf(L"Writing 1 byte\n");
		test(pipeStreamWrite(&pipeStreamState, data, 1) == 1);

		data[0] = 1;
		data[1] = 2;
		data[2] = 3;

		wprintf(L"Writing 3 bytes\n");
		test(pipeStreamWrite(&pipeStreamState, data, 3) == 1);

		deletePipeStreamState(&pipeStreamState);
	}
}

void test_IO()
{
	wprintf(L"Testing IO - PipeStream\n");
	test_IO_PipeStream();
}
