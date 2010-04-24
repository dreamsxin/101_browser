#include "TestSuite/Tests.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "GuiDataStructures/OuterBorderIterator.h"

void testIteratorInstances()
{
	std::vector<unsigned int> vect;
	vect.push_back(0);
	vect.push_back(1);
	vect.push_back(2);

	DoubleIteratorInstance<unsigned int, TriangleStripBorderIterator<unsigned int>::IteratorState> 
		tsbiInstance = triangleStripBorderIteratorInstance_create<unsigned int>(&vect);

	DoubleIteratorInstance<unsigned int, OuterBorderIterator<unsigned int>::IteratorState>
		obiInstance = outerBorderIteratorInstance_create<unsigned int>(&vect);

	std::vector<const unsigned int> constVect;
	constVect.push_back(0);
	constVect.push_back(1);
	constVect.push_back(2);

	DoubleIteratorInstance<const unsigned int, TriangleStripBorderIterator<const unsigned int>::ConstIteratorState> 
		tsbciInstance = triangleStripBorderConstIteratorInstance_create(&constVect);

	DoubleIteratorInstance<const unsigned int, OuterBorderIterator<const unsigned int>::ConstIteratorState>
		obciInstance = outerBorderConstIteratorInstance_create(&constVect);
}