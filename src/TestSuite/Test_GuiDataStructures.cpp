#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "TestSuite/TestSuite.h"

using namespace std;

void runIterator(TriangleStripBorderIteratorState<unsigned int>* in_pItState, 
				 Iterator<unsigned int, TriangleStripBorderIteratorState<unsigned int>> in_interface,
				 vector<unsigned int>* in_out_pResult)
{
	*in_out_pResult=vector<unsigned int>();

	while (!(*in_interface.mpfEnd)(in_pItState))
	{
		in_out_pResult->push_back(*(*in_interface.mpfGet)(in_pItState));
		(*in_interface.mpfNext)(in_pItState);
	}
}

bool compareVectors(const vector<unsigned int>* in_pV, const vector<unsigned int>* in_pW)
{
	test(in_pV->size()==in_pW->size());

	vector<unsigned int>::const_iterator i, j;

	for (i=in_pV->begin(), j=in_pW->begin(); i!=in_pV->end() && j!=in_pW->end(); i++, j++)
	{
		if (*i!=*j)
			return false;
	}

	return true;
}

void testGuiDataStructures()
{
	vector<unsigned int> v, w, result;
	TriangleStripBorderIteratorState<unsigned int> itState;
	Iterator<unsigned int, TriangleStripBorderIteratorState<unsigned int>> itInterface = 
		triangleStripBorderIterator_create<unsigned int>();

	// Testing correct behaviour for small datasets

	v=vector<unsigned int>();
	// Prepare v if necessary
	w=vector<unsigned int>();
	// Prepare w if necessary
	itState = triangleStripBorderIteratorState_create(&v);
	runIterator(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

	v=vector<unsigned int>();
	v.push_back(0);
	w=vector<unsigned int>();
	w.push_back(0);
	itState = triangleStripBorderIteratorState_create(&v);
	runIterator(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	itState = triangleStripBorderIteratorState_create(&v);
	runIterator(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(2);
	itState = triangleStripBorderIteratorState_create(&v);
	runIterator(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

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
	itState = triangleStripBorderIteratorState_create(&v);
	runIterator(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

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
	itState = triangleStripBorderIteratorState_create(&v);
	runIterator(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
}

