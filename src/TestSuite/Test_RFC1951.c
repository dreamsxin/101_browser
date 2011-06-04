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
#include <assert.h>

void fileOtherCoroutine(ByteStreamReference in_byteStreamReference, void *in_pUserData)
{
	size_t readCount;
	uint8_t buffer0, buffer1;

	test(in_byteStreamReference.mByteStreamInterface.mpfRead != NULL);

	if (NULL == in_byteStreamReference.mByteStreamInterface.mpfRead)
		return;

	while ((*in_byteStreamReference.mByteStreamInterface.mpfRead)
		(in_byteStreamReference.mpByteStreamState, &buffer0, 1) == 1)
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
	ByteStreamInterface pipeStreamInterface;

	CoroutineDescriptor thisCoroutine, otherCoroutine;
	FileByteStreamState rawFileByteStreamState, referenceFileByteStreamState;
	bool result;
	
	result = fileByteReadStreamStateInit(in_filenameRaw, 
		&rawFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = fileByteReadStreamStateInit(in_filenameReference, 
		&referenceFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface, 
		true, &thisCoroutine, &otherCoroutine, 
		fileOtherCoroutine, &referenceFileByteStreamState);
	test(result);

	if (!result)
		return;

	test(ReadResultOK == parseRFC1951(&rawFileByteStreamState, getFileByteStreamInterface(), 
		&pipeStreamState, pipeStreamInterface));

	/*
	* If we don't insert this code we won't change to the other coroutine
	* to see if we really sent all data.
	*/
	pipeStreamWrite(&pipeStreamState, NULL, 0);
	
	fileByteReadStreamStateDestroy(&referenceFileByteStreamState);
	fileByteReadStreamStateDestroy(&rawFileByteStreamState);
}

void zerosOtherCoroutine(ByteStreamReference in_byteStreamReference, void *in_pUserData)
{
	size_t readCount = 0;
	uint8_t buffer;

	test(in_byteStreamReference.mByteStreamInterface.mpfRead != NULL);

	if (NULL == in_byteStreamReference.mByteStreamInterface.mpfRead)
		return;

	while ((*in_byteStreamReference.mByteStreamInterface.mpfRead)
		(in_byteStreamReference.mpByteStreamState, &buffer, 1) == 1)
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
	ByteStreamInterface pipeStreamInterface;
	CoroutineDescriptor thisCoroutine, otherCoroutine;
	FileByteStreamState rawFileByteStreamState;
	bool result;
	
	result = fileByteReadStreamStateInit("testfiles/rfc1950_rfc1951/zeros.raw", 
		&rawFileByteStreamState);
	test(result);

	if (!result)
		return;

	result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface, 
		true, &thisCoroutine, &otherCoroutine, 
		&zerosOtherCoroutine, NULL);
	test(result);

	if (!result)
		return;

	test(ReadResultOK == parseRFC1951(&rawFileByteStreamState, getFileByteStreamInterface(), 
		&pipeStreamState, pipeStreamInterface));

	/*
	* If we don't insert this code we won't change to the other coroutine
	* to see if we really sent all data.
	*/
	pipeStreamWrite(&pipeStreamState, NULL, 0);

	fileByteReadStreamStateDestroy(&rawFileByteStreamState);
}

void test_RFC1951()
{
	test_RFC1951_zeros();

	test_RFC1951_File("testfiles/rfc1950_rfc1951/null.raw", "testfiles/rfc1950_rfc1951/null");
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random1024.raw", "testfiles/rfc1950_rfc1951/random1024");
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random65535.raw", "testfiles/rfc1950_rfc1951/random65535");
	test_RFC1951_File("testfiles/rfc1950_rfc1951/random65536.raw", "testfiles/rfc1950_rfc1951/random65536");
}
