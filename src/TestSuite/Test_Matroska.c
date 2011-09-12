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
	*
	* Note: unknownTests is an extended version of examples there.
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

	/*
	* Tests based on examples from
	* http://www.matroska.org/technical/specs/index.html
	*/
	uint8_t signedTest1_0[] = { 0xFE };
	uint8_t signedTest1_1[] = { 0x7F, 0xFE };
	uint8_t signedTest1_2[] = { 0x3F, 0xFF, 0xFE };
	uint8_t signedTest1_3[] = { 0x1F, 0xFF, 0xFF, 0xFE };
	uint8_t signedTest1_4[] = { 0x0F, 0xFF, 0xFF, 0xFF, 0xFE };
	uint8_t signedTest1_5[] = { 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE };
	uint8_t signedTest1_6[] = { 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE };
	uint8_t signedTest1_7[] = { 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE };

	uint8_t *signedTest1[] = { signedTest1_0, signedTest1_1, signedTest1_2, 
		signedTest1_3, signedTest1_4, signedTest1_5, signedTest1_6, signedTest1_7 };

	uint8_t signedTest2_0[] = { 0x85 };
	uint8_t signedTest2_1[] = { 0x40, 0x05 };
	uint8_t signedTest2_2[] = { 0x20, 0x00, 0x05 };
	uint8_t signedTest2_3[] = { 0x10, 0x00, 0x00, 0x05 };
	uint8_t signedTest2_4[] = { 0x08, 0x00, 0x00, 0x00, 0x05 };
	uint8_t signedTest2_5[] = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x05 };
	uint8_t signedTest2_6[] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05 };
	uint8_t signedTest2_7[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05 };
	
	uint8_t *signedTest2[] = { signedTest2_0, signedTest2_1, signedTest2_2, 
		signedTest2_3, signedTest2_4, signedTest2_5, signedTest2_6, signedTest2_7 };

	uint8_t elementID[4];
	uint64_t vint;
	int64_t svint;

	uint8_t idx;

	memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
		test0, sizeof(test0));
	memset(elementID, 0x42, 4);
	test(ReadResultOK == readEbmlElementID(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), elementID));
	test(0 == memcmp(elementID, test0, 3));
	test(0x42 == elementID[3]);

	memoryByteStreamReadStateReset(&memoryByteStreamReadState);
	vint = 0xDEADBEEFDEADBEEF;
	test(ReadResultOK == readEBml_vint(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), &vint));
	test(0x1A41FE == vint);

	memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
		test1, sizeof(test1));
	memset(elementID, 0x42, 4);
	test(ReadResultOK == readEbmlElementID(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), elementID));
	test(0 == memcmp(elementID, test1, 4));

	memoryByteStreamReadStateReset(&memoryByteStreamReadState);
	vint = 0xDEADBEEFDEADBEEF;
	test(ReadResultOK == readEBml_vint(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), &vint));
	test(0x1A41FE == vint);

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
	vint = 0xDEADBEEFDEADBEEF;
	test(ReadResultOK == readEBml_vint(&memoryByteStreamReadState, 
		getMemoryByteStreamReadInterface(), &vint));
	test(0x1A41FE == vint);

	// Now testing unknown lengths
	for (idx = 0; idx < 8; idx++)
	{
		memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
			unknownTests[idx], idx+1);
		test(ReadResultInvalidData == readEbmlElementID(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), elementID));

		memoryByteStreamReadStateReset(&memoryByteStreamReadState);
		vint = 0xDEADBEEFDEADBEEF;
		test(ReadResultOK == readEBml_vint(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), &vint));
		test(UINT64_MAX == vint);
	}

	// The first svint test
	for (idx = 0; idx < 8; idx++)
	{
		memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
			signedTest1[idx], idx+1);
		if (idx < 4)
			test(ReadResultOK == readEbmlElementID(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), elementID));
		else
			test(ReadResultInvalidData == readEbmlElementID(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), elementID));

		memoryByteStreamReadStateReset(&memoryByteStreamReadState);
		svint = 0xDEADBEEFDEADBEEF;
		test(ReadResultOK == readEBml_svint(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), &svint));
		test(-2 == svint);
	}

	// The second svint test
	for (idx = 0; idx < 8; idx++)
	{
		memoryByteStreamReadStateInit(&memoryByteStreamReadState, 
			signedTest2[idx], idx+1);
		if (idx < 4)
			test(ReadResultOK == readEbmlElementID(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), elementID));
		else
			test(ReadResultInvalidData == readEbmlElementID(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), elementID));

		memoryByteStreamReadStateReset(&memoryByteStreamReadState);
		vint = 0xDEADBEEFDEADBEEF;
		test(ReadResultOK == readEBml_vint(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), &vint));
		test(5 == vint);

		memoryByteStreamReadStateReset(&memoryByteStreamReadState);
		svint = 0xDEADBEEFDEADBEEF;
		test(ReadResultOK == readEBml_svint(&memoryByteStreamReadState, 
			getMemoryByteStreamReadInterface(), &svint));
		test(5 == svint);
	}
}

void test_Matroska()
{
	printf("Testing Matroska - EBML\n");
	test_EBML();
}
