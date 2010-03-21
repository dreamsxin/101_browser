#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

using namespace std;

void testTriangleStripBorderIterator()
{
	vector<unsigned int> v, w, result;
	TriangleStripBorderIterator<unsigned int>::IteratorState itState;
	DoubleIterator<unsigned int, TriangleStripBorderIterator<unsigned int>::IteratorState> 
		itInterface = triangleStripBorderIterator_create<unsigned int>();

	// Testing correct behaviour for small datasets

	v=vector<unsigned int>();
	w=vector<unsigned int>();
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	w=vector<unsigned int>();
	w.push_back(0);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(2);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	// Now the next two non-trivial cases

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(3);
	w.push_back(2);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(3);
	w.push_back(4);
	w.push_back(2);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);
}

