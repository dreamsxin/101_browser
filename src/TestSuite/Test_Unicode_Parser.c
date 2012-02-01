/*
 * Copyright 2008-2012 Wolfgang Keller
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

#include "IO/MemoryByteStream.h"
#include "IO/PipeStream.h"
#include "Unicode/Parser.h"
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include <assert.h>
#include <stdio.h>

typedef struct
{
	uint8_t *pInputBytes;
	MemoryByteStreamReadState *pReadState;
} Test_Parser_Userdata;

void test_UTF8_Coroutine(ByteStreamReference in_byteStreamReference, void *in_pUserData)
{
	Test_Parser_Userdata *pUserData = (Test_Parser_Userdata *) in_pUserData;
	ReadResult readResult;

	assert(in_byteStreamReference.mByteStreamInterface.mpfWrite != NULL);

	readResult = parse_UTF8(getMemoryByteStreamReadInterface(), 
		pUserData->pReadState, in_byteStreamReference.mByteStreamInterface, 
		in_byteStreamReference.mpByteStreamState);

	test(ReadResultOK == readResult);
}

void test_Unicode_Parser_UTF8()
{
	Test_Parser_Userdata test_UTF8_userdata;
	bool result;
	MemoryByteStreamReadState readState;
	PipeStreamState pipeStreamState;
	ByteStreamInterface pipeStreamInterface;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	size_t idx;
	UnicodeCodePoint currentCodePoint;
	size_t readCount;

	result = convertThreadToCoroutine(&thisCoroutine);

	test(result);
	if (!result)
		return;

	{
		/*
		* the test string from
		* http://www.whatwg.org/specs/web-apps/current-work/#utf-8
		* (24th of January 2011)
		*/
		uint8_t inputBytes[] = { 0x41, 0x98, 0xBA, 0x42, 0xE2, 0x98, 0x43, 0xE2, 0x98, 0xBA, 0xE2, 0x98 };
		UnicodeCodePoint outputCodepoints[] = { 'A', 0xFFFD, 0xFFFD, 'B', 0xFFFD, 'C', 0x263A, 0xFFFD };

		memoryByteStreamReadStateInit(&readState, inputBytes, sizeof(inputBytes));

		test_UTF8_userdata.pInputBytes = inputBytes;
		test_UTF8_userdata.pReadState = &readState;

		result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface,
			false, &thisCoroutine, &otherCoroutine,
			&test_UTF8_Coroutine, &test_UTF8_userdata);

		test(result);
		if (!result)
			return;

		test(pipeStreamInterface.mpfRead != NULL);
		if (NULL == pipeStreamInterface.mpfRead)
			return;

		for (idx = 0; idx < sizeof(outputCodepoints) / sizeof(UnicodeCodePoint); idx++)
		{
			readCount = (*pipeStreamInterface.mpfRead)
				(&pipeStreamState, &currentCodePoint, sizeof(UnicodeCodePoint));

			test(readCount == sizeof(UnicodeCodePoint));
			test(currentCodePoint == outputCodepoints[idx]);
		}

		readCount = pipeStreamRead(&pipeStreamState, &currentCodePoint, sizeof(UnicodeCodePoint));
		test(0 == readCount);

		pipeStreamStateDestroy(&pipeStreamState);
	}
}

void test_Unicode_Parser_UTF16()
{

}

void test_Unicode_Parser_UTF32()
{

}

void test_Unicode_Parser()
{
	printf("Testing Unicode Parser UTF8\n");
	test_Unicode_Parser_UTF8();

	printf("Testing Unicode Parser UTF16\n");
	test_Unicode_Parser_UTF16();

	printf("Testing Unicode Parser UTF16\n");
	test_Unicode_Parser_UTF32();
}
