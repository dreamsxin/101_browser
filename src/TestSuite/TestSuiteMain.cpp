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

#include <cstdio>
#include <cstring>
#include <cwchar>
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/MTAx_cstdlib.h" // for exit
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Network/Network.h"

int main()
{
#if 1
	printf("Testing Libc\n");
	test_Libc();
	printf("Testing MiniStdlib\n");
	test_MiniStdlib();
	printf("Testing Interval\n");
	testInterval();
	printf("Testing Algorithm\n");
	testAlgorithm();
	printf("Testing BasicDataStructures Tree\n");
	testBasicDataStructuresTree();
	printf("Testing TriangleStripBorderIterator\n");
	testTriangleStripBorderIterator();
	printf("Testing OuterBorderIterator\n");
	testOuterBorderIterator();
	printf("Testing BigNumber\n");
	testBigNumber();
	printf("Testing RFC 1950\n");
	test_RFC1950();
	printf("Testing RFC 1951\n");
	test_RFC1951();
	printf("Testing ASCIIStringUnicodeIterator\n");
	testASCIIStringUnicodeIterator();
	printf("Testing Unicode\n");
	testUnicode();
	printf("Testing IO\n");
	test_IO();
	printf("Testing HTML5\n");
	testHTML5();
	printf("Testing SetjmpUtil\n");
	testSetjmpUtil();
	printf("Testing GIF\n");
	testGIF();
	printf("Testing Coroutine\n");
	test_Coroutine();
	printf("Testing CRC\n");
	test_CRC();
	printf("Testing PNG\n");
	test_PNG();
	printf("Testing Matroska\n");
	test_Matroska();
#ifndef __linux
	printf("Testing Ogg\n");
	test_Ogg();
#endif
#endif
	if (startupNetwork())
	{
		fprintf(stderr, "Could not start network\n");
		exit(EXIT_FAILURE);
	}
	printf("Testing DNS\n");
	
	test_DNS();

	cleanupNetwork();

	return 0;
}
