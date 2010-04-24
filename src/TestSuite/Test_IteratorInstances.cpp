#include "TestSuite/Tests.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "GuiDataStructures/OuterBorderIterator.h"

void testIteratorInstances()
{
	std::vector<unsigned int> vect;
	for (unsigned int i=0; i<6; i++)
		vect.push_back(i);

	DoubleIteratorInstance<unsigned int, TriangleStripBorderIterator<unsigned int>::IteratorState> 
		tsbiInstance = triangleStripBorderIteratorInstance_create<unsigned int>(&vect);

	DoubleIteratorInstance<unsigned int, OuterBorderIterator<unsigned int>::IteratorState>
		obiInstance = outerBorderIteratorInstance_create<unsigned int>(&vect);

	std::vector<const unsigned int> constVect;
	for (unsigned int i=0; i<6; i++)
		constVect.push_back(i);

	DoubleIteratorInstance<const unsigned int, TriangleStripBorderIterator<const unsigned int>::ConstIteratorState> 
		tsbciInstance = triangleStripBorderConstIteratorInstance_create(&constVect);

	DoubleIteratorInstance<const unsigned int, OuterBorderIterator<const unsigned int>::ConstIteratorState>
		obciInstance = outerBorderConstIteratorInstance_create(&constVect);
}