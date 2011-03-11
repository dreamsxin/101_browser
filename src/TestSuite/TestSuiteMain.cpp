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
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"

int main()
{
	wprintf(L"Testing Interval\n");
	testInterval();
	wprintf(L"Testing Algorithm\n");
	testAlgorithm();
	wprintf(L"Testing BasicDataStructures Tree\n");
	testBasicDataStructuresTree();
	wprintf(L"Testing TriangleStripBorderIterator\n");
	testTriangleStripBorderIterator();
	wprintf(L"Testing OuterBorderIterator\n");
	testOuterBorderIterator();
	wprintf(L"Testing BigNumber\n");
	testBigNumber();
	wprintf(L"Testing RFC 1950\n");
	test_RFC1950();
	wprintf(L"Testing ASCIIStringUnicodeIterator\n");
	testASCIIStringUnicodeIterator();
	wprintf(L"Testing Unicode\n");
	testUnicode();
	wprintf(L"Testing IO\n");
	test_IO();
	wprintf(L"Testing HTML5\n");
	testHTML5();
	wprintf(L"Testing GIF\n");
	testGIF();
	wprintf(L"Testing CPU\n");
	testCPU();
	wprintf(L"Testing Coroutine\n");
	test_Coroutine();
	wprintf(L"Testing PNG\n");
	test_PNG();

	return 0;
}
