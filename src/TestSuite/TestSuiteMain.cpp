#include <cstdio>
#include "TestSuite/Tests.h"

int main()
{
	testBasicDataStructures();
	testBasicDataStructuresTree();
	testTriangleStripBorderIterator();

	testOuterBorderIterator<
		unsigned int,
		std::vector<unsigned int>,
		OuterBorderIterator<unsigned int>::IteratorState,
		DoubleIterator<unsigned int, OuterBorderIterator<unsigned int>::IteratorState> >(
		&outerBorderIteratorState_create<unsigned int>, 
		&outerBorderIterator_create<unsigned int>
		);
	testOuterBorderIterator<
		const unsigned int,
		const std::vector<const unsigned int>,
		OuterBorderIterator<const unsigned int>::ConstIteratorState,
		DoubleIterator<const unsigned int, OuterBorderIterator<const unsigned int>::ConstIteratorState> >(
		&outerBorderConstIteratorState_create<const unsigned int>, 
		&outerBorderConstIterator_create<const unsigned int>
		);

	return 0;
}
