#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

using namespace std;

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

void reverseVector(vector<unsigned int>* in_pV)
{
	for (size_t i=0; i<in_pV->size()/2; i++)
	{
		std::swap(in_pV->at(i), in_pV->at(in_pV->size()-1-i));
	}
}
