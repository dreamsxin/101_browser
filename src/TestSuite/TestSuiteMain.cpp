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
	wprintf(L"Testing rfc1950\n");
	testRfc1950();
	wprintf(L"Testing ASCIIStringUnicodeIterator\n");
	testASCIIStringUnicodeIterator();
	wprintf(L"Testing Unicode\n");
	testUnicode();
	wprintf(L"Testing HTML5\n");
	testHTML5();
	wprintf(L"Testing GIF\n");
	testGIF();
	wprintf(L"Testing CPU\n");
	testCPU();
	wprintf(L"Testing PNG\n");
	test_PNG();

	return 0;
}
