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

#include "IO/MemoryByteStream_v4.h"
#include "Unicode/Parser.h"
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include <assert.h>
#include <stdio.h>
#include <string.h> // memcmp

void test_Unicode_Parser_UTF8()
{
#if 0
	MemoryByteStreamReadState readState;
	ByteStreamInterface pipeStreamInterface;
	size_t idx;
	UnicodeCodePoint currentCodePoint;
	size_t readCount;

	/*
	* the test string from
	* http://www.whatwg.org/specs/web-apps/current-work/#utf-8
	* (24th of January 2011)
	*/
	uint8_t bufferIn[] = { 0x41, 0x98, 0xBA, 0x42, 0xE2, 0x98, 0x43, 0xE2, 0x98, 0xBA, 0xE2, 0x98 };
	UnicodeCodePoint result[] = { 'A', 0xFFFD, 0xFFFD, 'B', 0xFFFD, 'C', 0x263A, 0xFFFD };

	ByteStreamReadInterface_v4 readInterface = memoryByteStreamReadInterface_v4_get();
	ByteStreamWriteInterface_v4 writeInterface = memoryByteStreamWriteInterface_v4_get();

	{
		

		memoryByteStreamReadStateInit(&readState, inputBytes, sizeof(inputBytes));

		test_UTF8_userdata.pInputBytes = inputBytes;
		test_UTF8_userdata.pReadState = &readState;

#if 0
		result = pipeStreamInit(&pipeStreamState, &pipeStreamInterface,
			false, &thisCoroutine, &otherCoroutine,
			&test_UTF8_Coroutine, &test_UTF8_userdata);
#endif

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
	}
#endif
}

void test_Unicode_Parser_UTF16()
{
	UnicodeCodePoint bufferOut[32];
	MemoryByteStream_v4State readState;
	MemoryByteStream_v4State writeState;
	UTF16_State utf16State;
	uint8_t triggerAsEarlyAsPossible;
	UnicodeCodePoint singleCodePoint;

	const UnicodeCodePoint result[] = {
		0x00FF, 0x2020, 0xD7FF, 

		// Replacement character because of single High Surrogate
		0xFFFD, 

		// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
		0x007A, 0x6C34, 0x10000, 0x1D11E, 0x10FFFD, 

		// Replacement character because of single Low Surrogate
		0xFFFD
	};

	ParseBlocker parseBlocker;
	size_t allReadCount;

	ByteStreamReadInterface_v4 readInterface = memoryByteStreamReadInterface_v4_get();
	ByteStreamWriteInterface_v4 writeInterface = memoryByteStreamWriteInterface_v4_get();
	
	for (triggerAsEarlyAsPossible = 0; triggerAsEarlyAsPossible < 2; triggerAsEarlyAsPossible++)
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
				0xDB, 0xFF, 0xDF, 0xFD, 

				// Single Low Surrogate
				0xDC, 0x00
			};

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf16_StateInit(&utf16State, true);

			allReadCount = 0;

			while ((parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				assert(allReadCount < sizeof(result) / sizeof(UnicodeCodePoint));
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;
			
			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v4StateReset(&readState);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf16_StateReset(&utf16State);
			
			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
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
				0xFF, 0xDB, 0xFD, 0xDF, 

				// Single Low Surrogate
				0x00, 0xDC
			};

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf16_StateInit(&utf16State, false);

			allReadCount = 0;

			while ((parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				assert(allReadCount < sizeof(result) / sizeof(UnicodeCodePoint));
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v4StateReset(&readState);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf16_StateReset(&utf16State);

			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
			test(!memcmp(result, bufferOut, sizeof(result)));
		}
	}
}

void test_Unicode_Parser_UTF32()
{
	UnicodeCodePoint bufferOut[32];
	MemoryByteStream_v4State readState;
	MemoryByteStream_v4State writeState;
	UTF32_State utf32State;
	uint8_t triggerAsEarlyAsPossible;
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
	
	ParseBlocker parseBlocker;
	size_t allReadCount;

	ByteStreamReadInterface_v4 readInterface = memoryByteStreamReadInterface_v4_get();
	ByteStreamWriteInterface_v4 writeInterface = memoryByteStreamWriteInterface_v4_get();

	for (triggerAsEarlyAsPossible = 0; triggerAsEarlyAsPossible < 2; triggerAsEarlyAsPossible++)
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

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf32_StateInit(&utf32State, true);

			allReadCount = 0;

			while ((parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				assert(allReadCount < sizeof(result) / sizeof(UnicodeCodePoint));
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;
			
			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v4StateReset(&readState);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf32_StateReset(&utf32State);
			
			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
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

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf32_StateInit(&utf32State, false);

			allReadCount = 0;

			while ((parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				assert(allReadCount < sizeof(result) / sizeof(UnicodeCodePoint));
				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;
			
			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			memoryByteStream_v4StateReset(&readState);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf32_StateReset(&utf32State);

			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
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
