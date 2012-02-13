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
#include "IO/MemoryByteStream_v3.h"
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
	UnicodeCodePoint out_buffer[32];
	MemoryByteStream_v3State readState;
	MemoryByteStream_v3State writeState;
	size_t idx;
	uint8_t terminateAfterLastOperation;
	ReadResult readResult;

	const UnicodeCodePoint result[] = {
		0x00FF, 0x2020, 0xD7FF, 

		// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
		0x007A, 0x6C34, 0x10000, 0x1D11E, 0x10FFFD

	};
	
	for (terminateAfterLastOperation = 0; terminateAfterLastOperation < 2; terminateAfterLastOperation++)
	{
		memoryByteStream_v3WriteStateInit(&writeState, out_buffer, sizeof(out_buffer), false);

		// Big Endian
		{
			const uint8_t in_buffer[] = {
				0x00, 0xFF, 
				0x20, 0x20, 
				0xD7, 0xFF, 

				// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
				0x00, 0x7A, 
				0x6C, 0x34, 
				0xD8, 0x00, 0xDC, 0x00, 
				0xD8, 0x34, 0xDD, 0x1E, 
				0xDB, 0xFF, 0xDF, 0xFD
			};

			memoryByteStream_v3StateReset(&writeState);

			memoryByteStream_v3ReadStateInit(&readState, in_buffer, sizeof(in_buffer), 
				(bool) terminateAfterLastOperation);

			readResult = parse_UTF16(memoryByteStreamReadInterface_v3_get(), &readState,
				memoryByteStreamWriteInterface_v3_get(), &writeState, true);
			
			test(ReadResultOK == readResult);
			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);

			test(sizeof(result) == writeState.bufferPos);

			for (idx = 0; idx < sizeof(result)/sizeof(UnicodeCodePoint); idx++)
			{
				test(result[idx] == out_buffer[idx]);
			}
		}

		// Little Endian
		{
			const uint8_t in_buffer[] = {
				0xFF, 0x00, 
				0x20, 0x20, 
				0xFF, 0xD7, 

				// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
				0x7A, 0x00, 
				0x34, 0x6C, 
				0x00, 0xD8, 0x00, 0xDC, 
				0x34, 0xD8, 0x1E, 0xDD, 
				0xFF, 0xDB, 0xFD, 0xDF
			};

			memoryByteStream_v3StateReset(&writeState);

			memoryByteStream_v3ReadStateInit(&readState, in_buffer, sizeof(in_buffer), 
				(bool) terminateAfterLastOperation);

			readResult = parse_UTF16(memoryByteStreamReadInterface_v3_get(), &readState,
				memoryByteStreamWriteInterface_v3_get(), &writeState, false);

			test(ReadResultOK == readResult);
			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);

			test(sizeof(result) == writeState.bufferPos);

			for (idx = 0; idx < sizeof(result)/sizeof(UnicodeCodePoint); idx++)
			{
				test(result[idx] == out_buffer[idx]);
			}
		}
	}
}

void test_Unicode_Parser_UTF32()
{
	
	
	// Little Endian

	
	const uint8_t in_buffer[] = { 0x00 };
	UnicodeCodePoint out_buffer[32];
	MemoryByteStream_v3State read_state;
	MemoryByteStream_v3State write_state;

	memoryByteStream_v3ReadStateInit(&read_state, in_buffer, sizeof(in_buffer), false);
	memoryByteStream_v3WriteStateInit(&write_state, out_buffer, sizeof(out_buffer), false);
	
	// Big Endian


	// Little Endian

}

void test_Unicode_Parser()
{
	printf("Testing Unicode Parser UTF8\n");
	test_Unicode_Parser_UTF8();

	printf("Testing Unicode Parser UTF16\n");
	test_Unicode_Parser_UTF16();

	printf("Testing Unicode Parser UTF32\n");
	test_Unicode_Parser_UTF32();
}
