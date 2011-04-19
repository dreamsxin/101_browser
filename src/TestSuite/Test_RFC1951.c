/*
* Copyright 2011 Wolfgang Keller
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

#include "RFC1951/RFC1951.h"
#include "IO/FileByteStream.h"
#include "IO/PipeStream.h"

void zerosOtherCoroutine(void *in_out_pStreamState, void *in_pUserData)
{
	size_t readCount = 0;
	uint8_t buffer;

	while (pipeStreamRead(in_out_pStreamState, &buffer, 1) == 1)
	{
		readCount++;
	}
}

void test_RFC1951_File(const char *in_filenameRaw, const char *in_filenameReference)
{
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine, otherCoroutine;
	FileByteStreamState rawFileByteStreamState;
	bool result;
	
	result = fileByteReadStreamState_create(in_filenameRaw, 
		&rawFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = initPipeStreamState(&pipeStreamState, true, &thisCoroutine, &otherCoroutine, 
		&zerosOtherCoroutine, NULL);
	test(result);

	if (!result)
		return;

	test(ReadResultOK == parseRFC1951(&rawFileByteStreamState, cFileByteStreamInterface, 
		&pipeStreamState, getPipeStreamWriteInterface()));

	fileByteReadStreamState_destroy(&rawFileByteStreamState);
}

void test_RFC1951_zeros()
{
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine, otherCoroutine;
	FileByteStreamState rawFileByteStreamState;
	bool result;
	
	result = fileByteReadStreamState_create("testfiles/rfc1950_rfc1951/zeros.raw", 
		&rawFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = initPipeStreamState(&pipeStreamState, true, &thisCoroutine, &otherCoroutine, 
		&zerosOtherCoroutine, NULL);
	test(result);

	if (!result)
		return;

	test(ReadResultOK == parseRFC1951(&rawFileByteStreamState, cFileByteStreamInterface, 
		&pipeStreamState, getPipeStreamWriteInterface()));

	fileByteReadStreamState_destroy(&rawFileByteStreamState);
}

void test_RFC1951()
{
	test_RFC1951_zeros();
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random1024.raw", "testfiles/rfc1950_rfc1951/random1024");
}
