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

void fileOtherCoroutine(void *in_out_pStreamState, void *in_pUserData)
{
	size_t readCount;
	uint8_t buffer0, buffer1;

	while (pipeStreamRead(in_out_pStreamState, &buffer0, 1) == 1)
	{
		readCount = fileByteReadStreamRead(in_pUserData, &buffer1, 1);
		if (readCount != 1)
		{
			test(readCount == 1);
		}
		if (buffer0 != buffer1)
		{
			test(buffer0 == buffer1);
		}
	}

	test(fileByteReadStreamRead(in_pUserData, &buffer1, 1) == 0);
}

void test_RFC1951_File(const char *in_filenameRaw, const char *in_filenameReference)
{
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine, otherCoroutine;
	FileByteStreamState rawFileByteStreamState, referenceFileByteStreamState;
	bool result;
	
	result = fileByteReadStreamState_create(in_filenameRaw, 
		&rawFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = fileByteReadStreamState_create(in_filenameReference, 
		&referenceFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = pipeStreamInit(&pipeStreamState, true, &thisCoroutine, &otherCoroutine, 
		fileOtherCoroutine, &referenceFileByteStreamState);
	test(result);

	if (!result)
		return;

	test(ReadResultOK == parseRFC1951(&rawFileByteStreamState, cFileByteStreamInterface, 
		&pipeStreamState, getPipeStreamWriteInterface()));

	/*
	* If we don't insert this code we won't change to the other coroutine
	* to see if we really sent all data.
	*/
	pipeStreamWrite(&pipeStreamState, NULL, 0);
	
	fileByteReadStreamState_destroy(&referenceFileByteStreamState);
	fileByteReadStreamState_destroy(&rawFileByteStreamState);
}

void zerosOtherCoroutine(void *in_out_pStreamState, void *in_pUserData)
{
	size_t readCount = 0;
	uint8_t buffer;

	while (pipeStreamRead(in_out_pStreamState, &buffer, 1) == 1)
	{
		readCount++;

		if (buffer != 0x00)
			test(0x00 == buffer);
	}

	/*
	* The code for parsing of deflate data is incomplete, so I commented
	* this testcase.
	*/
#if 0
	printf("Testing whether we have read the correct amount of 0x00 bytes.\n");
	test(1234567 == readCount);
#endif
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

	result = pipeStreamInit(&pipeStreamState, true, &thisCoroutine, &otherCoroutine, 
		&zerosOtherCoroutine, NULL);
	test(result);

	if (!result)
		return;

	test(ReadResultOK == parseRFC1951(&rawFileByteStreamState, cFileByteStreamInterface, 
		&pipeStreamState, getPipeStreamWriteInterface()));

	/*
	* If we don't insert this code we won't change to the other coroutine
	* to see if we really sent all data.
	*/
	pipeStreamWrite(&pipeStreamState, NULL, 0);

	fileByteReadStreamState_destroy(&rawFileByteStreamState);
}

void test_RFC1951()
{
	test_RFC1951_zeros();

	test_RFC1951_File("testfiles/rfc1950_rfc1951/null.raw", "testfiles/rfc1950_rfc1951/null");
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random1024.raw", "testfiles/rfc1950_rfc1951/random1024");
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random65535.raw", "testfiles/rfc1950_rfc1951/random65535");
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random65536.raw", "testfiles/rfc1950_rfc1951/random65536");
}
