#include <cstdio>
#include "TestSuite/Tests.h"

int main()
{
	testBasicDataStructures();
	testBasicDataStructuresTree();
	printf("Testing TriangleStripBorderIterator\n");
	testTriangleStripBorderIterator();
	printf("Testing OuterBorderIterator\n");
	testOuterBorderIterator();
	printf("Testing rfc1950\n");
	testRfc1950();

	return 0;
}
