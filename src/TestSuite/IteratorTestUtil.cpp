#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

using namespace std;

void reverseVector(vector<unsigned int>* in_pV)
{
	for (size_t i=0; i<in_pV->size()/2; i++)
	{
		std::swap(in_pV->at(i), in_pV->at(in_pV->size()-1-i));
	}
}

