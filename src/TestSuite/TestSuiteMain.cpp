#include <cstdio>
#include "TestSuite/Tests.h"

int main()
{
	testBasicDataStructures();
	testBasicDataStructuresTree();
	testTriangleStripBorderIterator();
	testOuterBorderIterator<OuterBorderIterator<unsigned int>::IteratorState,
		DoubleIterator<unsigned int, OuterBorderIterator<unsigned int>::IteratorState> >(
		&outerBorderIterator_create<unsigned int>
		);
	/*testOuterBorderIterator<OuterBorderIterator<const unsigned int>::ConstIteratorState,
		DoubleIterator<const unsigned int, OuterBorderIterator<const unsigned int>::ConstIteratorState> >(
		&outerBorderConstIterator_create<const unsigned int>
		);*/

	return 0;
}
