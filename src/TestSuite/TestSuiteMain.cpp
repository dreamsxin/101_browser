#include <cstdio>
#include "TestSuite/Tests.h"

int main()
{
	testBasicDataStructures();
	testBasicDataStructuresTree();

	testTriangleStripBorderIterator
		<unsigned int, 
		std::vector<unsigned int>,
		TriangleStripBorderIterator<unsigned int>::IteratorState>
		(
		&triangleStripBorderIteratorState_create<unsigned int>, 
		&triangleStripBorderIterator_create<unsigned int>
		);

#if 1
	testTriangleStripBorderIterator
		<const unsigned int, 
		const std::vector<const unsigned int>,
		TriangleStripBorderIterator<const unsigned int>::ConstIteratorState>
		(
		&triangleStripBorderConstIteratorState_create<const unsigned int>, 
		&triangleStripBorderConstIterator_create<const unsigned int>
		);
#endif

	testOuterBorderIterator
		<unsigned int,
		std::vector<unsigned int>,
		OuterBorderIterator<unsigned int>::IteratorState>
		(
		&outerBorderIteratorState_create<unsigned int>, 
		&outerBorderIterator_create<unsigned int>
		);

	testOuterBorderIterator
		<const unsigned int,
		const std::vector<const unsigned int>,
		OuterBorderIterator<const unsigned int>::ConstIteratorState>
		(
		&outerBorderConstIteratorState_create<const unsigned int>, 
		&outerBorderConstIterator_create<const unsigned int>
		);

	return 0;
}
