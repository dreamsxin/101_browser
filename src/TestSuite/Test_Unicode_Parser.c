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
	UnicodeCodePoint bufferOut[32];
	MemoryByteStream_v3State readState;
	UTF16_State utf16State;
	uint8_t terminateAfterLastOperation;
	UnicodeCodePoint singleCodePoint;

	const UnicodeCodePoint result[] = {
		0x00FF, 0x2020, 0xD7FF, 

		// Replacement character because of single high surrogate
		0xFFFD, 

		// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
		0x007A, 0x6C34, 0x10000, 0x1D11E, 0x10FFFD
	};

	size_t currentReadCount;
	size_t allReadCount;
	ByteStreamReadInterface_v3 utf16Interface = getUTF16_ReadInterface();
	
	for (terminateAfterLastOperation = 0; terminateAfterLastOperation < 2; terminateAfterLastOperation++)
	{
		// Big Endian
		{
			const uint8_t bufferIn[] = {
				0x00, 0xFF, 
				0x20, 0x20, 
				0xD7, 0xFF, 

				// Single High Surrogate
				0xD8, 0x00, 

				// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
				0x00, 0x7A, 
				0x6C, 0x34, 
				0xD8, 0x00, 0xDC, 0x00, 
				0xD8, 0x34, 0xDD, 0x1E, 
				0xDB, 0xFF, 0xDF, 0xFD
			};

			memoryByteStream_v3ReadStateInit(&readState, bufferIn, sizeof(bufferIn), 
				(bool) terminateAfterLastOperation);

			utf16_StateInit(&utf16State, memoryByteStreamReadInterface_v3_get(), 
				&readState, true);

			allReadCount = 0;

			while ((currentReadCount = utf16Interface.mpfRead(&utf16State, &singleCodePoint, 1)) != 0)
			{
				test(1 == currentReadCount);
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}
			
			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf16Interface.commonByteStreamInterface.mpfIsTerminated(&utf16State));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v3StateReset(&readState);
			utf16_StateReset(&utf16State);

			allReadCount = utf16Interface.mpfRead(&utf16State, 
				bufferOut, sizeof(bufferOut) / sizeof(UnicodeCodePoint));

			test(sizeof(result) == allReadCount * sizeof(UnicodeCodePoint));

			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf16Interface.commonByteStreamInterface.mpfIsTerminated(&utf16State));
			test(!memcmp(result, bufferOut, sizeof(result)));
		}

		// Little Endian
		{
			const uint8_t bufferIn[] = {
				0xFF, 0x00, 
				0x20, 0x20, 
				0xFF, 0xD7, 
				
				// Single High Surrogate
				0x00, 0xD8, 

				// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
				0x7A, 0x00, 
				0x34, 0x6C, 
				0x00, 0xD8, 0x00, 0xDC, 
				0x34, 0xD8, 0x1E, 0xDD, 
				0xFF, 0xDB, 0xFD, 0xDF
			};

			memoryByteStream_v3ReadStateInit(&readState, bufferIn, sizeof(bufferIn), 
				(bool) terminateAfterLastOperation);

			utf16_StateInit(&utf16State, memoryByteStreamReadInterface_v3_get(), 
				&readState, false);

			allReadCount = 0;

			while ((currentReadCount = utf16Interface.mpfRead(&utf16State, &singleCodePoint, 1)) != 0)
			{
				test(1 == currentReadCount);
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf16Interface.commonByteStreamInterface.mpfIsTerminated(&utf16State));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v3StateReset(&readState);
			utf16_StateReset(&utf16State);

			allReadCount = utf16Interface.mpfRead(&utf16State, 
				bufferOut, sizeof(bufferOut) / sizeof(UnicodeCodePoint));

			test(sizeof(result) == allReadCount * sizeof(UnicodeCodePoint));

			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf16Interface.commonByteStreamInterface.mpfIsTerminated(&utf16State));
			test(!memcmp(result, bufferOut, sizeof(result)));
		}
	}
}

void test_Unicode_Parser_UTF32()
{
	UnicodeCodePoint bufferOut[32];
	MemoryByteStream_v3State readState;
	UTF32_State utf32State;
	uint8_t terminateAfterLastOperation;
	UnicodeCodePoint singleCodePoint;

	const UnicodeCodePoint result[] = {
		0x00, 
		0xD7FF, 
		0xFFFD, // replacement character
		0xFFFD, // replacement character
		0xE000, 
		0x10FFFF, 
		0xFFFD, // replacement character
		0xFFFD, // replacement character
		0xFFFD  // replacement character
	};

	size_t currentReadCount;
	size_t allReadCount;
	ByteStreamReadInterface_v3 utf32Interface = getUTF32_ReadInterface();

	for (terminateAfterLastOperation = 0; terminateAfterLastOperation < 2; terminateAfterLastOperation++)
	{
		// Big Endian
		{
			const uint8_t bufferIn[] = {
				0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0xD7, 0xFF, 
				0x00, 0x00, 0xD8, 0x00, 
				0x00, 0x00, 0xDF, 0xFF, 
				0x00, 0x00, 0xE0, 0x00, 
				0x00, 0x10, 0xFF, 0xFF, 
				0x00, 0x11, 0x00, 0x00, 
				0xFF, 0xFF, 0xFF, 0xFF, 
				0x00, 0x00, 0x00
			};

			memoryByteStream_v3ReadStateInit(&readState, bufferIn, sizeof(bufferIn), 
				(bool) terminateAfterLastOperation);

			utf32_StateInit(&utf32State, memoryByteStreamReadInterface_v3_get(), 
				&readState, true);

			allReadCount = 0;

			while ((currentReadCount = utf32Interface.mpfRead(&utf32State, &singleCodePoint, 1)) != 0)
			{
				test(1 == currentReadCount);
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}
			
			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf32Interface.commonByteStreamInterface.mpfIsTerminated(&utf32State));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v3StateReset(&readState);

			allReadCount = utf32Interface.mpfRead(&utf32State, 
				bufferOut, sizeof(bufferOut) / sizeof(UnicodeCodePoint));

			test(sizeof(result) == allReadCount * sizeof(UnicodeCodePoint));

			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf32Interface.commonByteStreamInterface.mpfIsTerminated(&utf32State));
			test(!memcmp(result, bufferOut, sizeof(result)));
		}

		// Little Endian
		{
			const uint8_t bufferIn[] = {
				0x00, 0x00, 0x00, 0x00, 
				0xFF, 0xD7, 0x00, 0x00, 
				0x00, 0xD8, 0x00, 0x00, 
				0xFF, 0xDF, 0x00, 0x00, 
				0x00, 0xE0, 0x00, 0x00, 
				0xFF, 0xFF, 0x10, 0x00, 
				0x00, 0x00, 0x11, 0x00, 
				0xFF, 0xFF, 0xFF, 0xFF, 
				0x20, 0x00, 0x00
			};

			memoryByteStream_v3ReadStateInit(&readState, bufferIn, sizeof(bufferIn), 
				(bool) terminateAfterLastOperation);

			utf32_StateInit(&utf32State, memoryByteStreamReadInterface_v3_get(), 
				&readState, false);

			allReadCount = 0;

			while ((currentReadCount = utf32Interface.mpfRead(&utf32State, &singleCodePoint, 1)) != 0)
			{
				test(1 == currentReadCount);
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}
			
			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf32Interface.commonByteStreamInterface.mpfIsTerminated(&utf32State));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v3StateReset(&readState);

			allReadCount = utf32Interface.mpfRead(&utf32State, 
				bufferOut, sizeof(bufferOut) / sizeof(UnicodeCodePoint));

			test(sizeof(result) == allReadCount * sizeof(UnicodeCodePoint));

			test(readState.bufferPos == readState.bufferSize);
			test(readState.isTerminated);
			test(utf32Interface.commonByteStreamInterface.mpfIsTerminated(&utf32State));
			test(!memcmp(result, bufferOut, sizeof(result)));
		}
	}
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
