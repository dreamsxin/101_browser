#include "TestSuite/Tests.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "GuiDataStructures/OuterBorderIterator.h"

void testIteratorInstances()
{
	std::vector<unsigned int> vect;
	for (unsigned int i=0; i<6; i++)
		vect.push_back(i);

	DoubleIteratorInstance<TriangleStripBorderIterator<unsigned int>::IteratorState>
		tsbiInstance = triangleStripBorderIteratorInstance_create<unsigned int>(&vect);

	DoubleIteratorInstance<OuterBorderIterator<unsigned int>::IteratorState>
		obiInstance = outerBorderIteratorInstance_create<unsigned int>(&vect);
}
