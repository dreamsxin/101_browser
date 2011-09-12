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
#include "Matroska/EBML.h"
#include "IO/MemoryByteStream.h"
#include "MiniStdlib/cstring.h"
#include "MiniStdlib/MTAx_cstdio.h"

void test_EBML()
{
	/*
	* Tests taken from http://matroska.org/files/matroska.pdf
	* "2.1 Unsigned Integer Values of Variable Length (”vint“)"
	*/
	MemoryByteStreamReadState memoryByteStreamReadState;
	uint8_t test0[] = { 0x3A, 0x41, 0xFE };
	uint8_t test1[] = { 0x10, 0x1A, 0x41, 0xFE };
	uint8_t test2[] = { 0x08, 0x00, 0x1A, 0x41, 0xFE };

	uint8_t unknownTest0[] = { 0xFF };
	uint8_t unknownTest1[] = { 0x7F, 0xFF };
	uint8_t unknownTest2[] = { 0x3F, 0xFF, 0xFF };
	uint8_t unknownTest3[] = { 0x1F, 0xFF, 0xFF, 0xFF };
	uint8_t unknownTest4[] = { 0x0F, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t unknownTest5[] = { 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t unknownTest6[] = { 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t unknownTest7[] = { 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	uint8_t *unknownTests[] = { unknownTest0, unknownTest1, unknownTest2, 
		unknownTest3, unknownTest4, unknownTest5, unknownTest6, unknownTest7 };

	uint8_t elementID[4];
	uint64_t elementSize;

	uint8_t idx;

	memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
		test0, sizeof(test0));
	memset(elementID, 0x42, 4);
	test(ReadResultOK == readEbmlElementID(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), elementID));
	test(0 == memcmp(elementID, test0, 3));
	test(0x42 == elementID[3]);

	memoryByteStreamReadStateReset(&memoryByteStreamReadState);
	elementSize = 0xDEADBEEFDEADBEEF;
	test(ReadResultOK == readEBmlElementSize(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), &elementSize));
	test(0x1A41FE == elementSize);

	memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
		test1, sizeof(test1));
	memset(elementID, 0x42, 4);
	test(ReadResultOK == readEbmlElementID(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), elementID));
	test(0 == memcmp(elementID, test1, 4));

	memoryByteStreamReadStateReset(&memoryByteStreamReadState);
	elementSize = 0xDEADBEEFDEADBEEF;
	test(ReadResultOK == readEBmlElementSize(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), &elementSize));
	test(0x1A41FE == elementSize);

	memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
		test2, sizeof(test2));
#if 0
	memset(elementID, 0x42, 4);
#endif
	test(ReadResultInvalidData == readEbmlElementID(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), elementID));
	/*
	* Remark: we don't do a test as (Pseudo-Code!):
	* test(0 == memcmp(elementID, { 0x42, 0x42, 0x42, 0x42 }, 4));
	* since you may not make any assumptions about the buffer passed to 
	* readEbmlElementID if not ReadResultOK is returned.
	*/

	memoryByteStreamReadStateReset(&memoryByteStreamReadState);
	elementSize = 0xDEADBEEFDEADBEEF;
	test(ReadResultOK == readEBmlElementSize(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), &elementSize));
	test(0x1A41FE == elementSize);

	// Now testing unknown lengths
	for (idx = 0; idx < 8; idx++)
	{
		memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
			unknownTests[idx], idx+1);
		test(ReadResultInvalidData == readEbmlElementID(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), elementID));

		memoryByteStreamReadStateReset(&memoryByteStreamReadState);
		elementSize = 0xDEADBEEFDEADBEEF;
		test(ReadResultOK == readEBmlElementSize(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), &elementSize));
		test(UINT64_MAX == elementSize);
	}
}

void test_Matroska()
{
	printf("Testing Matroska - EBML\n");
	test_EBML();
}
