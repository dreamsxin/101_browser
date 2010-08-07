#include <cstdio>
#include <cstring>
#include "TestSuite/Tests.h"

int main()
{
	testBasicDataStructures();
	testBasicDataStructuresTree();
	printf("Testing TriangleStripBorderIterator\n");
	testTriangleStripBorderIterator();
	printf("Testing OuterBorderIterator\n");
	testOuterBorderIterator();
	printf("Testing BigNumber\n");
	testBigNumber();
	printf("Testing rfc1950\n");
	testRfc1950();
	printf("Testing ASCIIStringUnicodeIterator\n");
	testASCIIStringUnicodeIterator();
	printf("Testing HTML5\n");
	testHTML5();

	return 0;
}
