#include "GuiDataStructures/OuterBorderIterator.h"
#include "TestSuite/TestSuite.h"
using namespace std;

void testOuterBorderIterator()
{
	vector<unsigned int> v, w, result;
	OuterBorderIterator<unsigned int>::IteratorState itState;

	v=std::vector<unsigned int>();
	v.push_back(0);
}