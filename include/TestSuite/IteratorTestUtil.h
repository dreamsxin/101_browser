#ifndef _IteratorTestUtil_h
#define _IteratorTestUtil_h

#include <vector>
#include "BasicDataStructures/Iterator.h"
#include "TestSuite/TestSuite.h"

template <typename IteratorState> 
void runIteratorForward(IteratorState* in_pItState, 
						DoubleIterator<unsigned int, IteratorState> in_interface,
						std::vector<unsigned int>* in_out_pResult)
{
	*in_out_pResult=std::vector<unsigned int>();

	if ((*in_interface.mpfGet)(in_pItState) == NULL)
	{
		test((*in_interface.mpfIterateNext)(in_pItState) == IterateResultToInvalidState);
		return;
	}

	IterateResult itRes;

	do
	{
		in_out_pResult->push_back(*(*in_interface.mpfGet)(in_pItState));
		itRes = (*in_interface.mpfIterateNext)(in_pItState);
	}
	while (itRes == IterateResultOK);

	test(itRes == IterateResultOverBoundary);
}

template <typename IteratorState> 
void runIteratorBackward(IteratorState* in_pItState, 
						 DoubleIterator<unsigned int, IteratorState> in_interface,
						 std::vector<unsigned int>* in_out_pResult)
{
	*in_out_pResult=std::vector<unsigned int>();

	IterateResult itRes = in_interface.mpfIteratePrev(in_pItState);

	if (itRes == IterateResultToInvalidState)
	{
		test(in_interface.mpfGet(in_pItState)==NULL);
		return;
	}

	test(itRes == IterateResultOverBoundary);

	do
	{
		in_out_pResult->push_back(*(*in_interface.mpfGet)(in_pItState));
		itRes = (*in_interface.mpfIteratePrev)(in_pItState);
	}
	while (itRes == IterateResultOK);

	test(itRes == IterateResultOverBoundary);
}

bool compareVectors(const std::vector<unsigned int>* in_pV, 
					const std::vector<unsigned int>* in_pW);
void reverseVector(std::vector<unsigned int>* in_pV);

template <typename IteratorState> void testIterator(
	std::vector<unsigned int>& v, 
	std::vector<unsigned int>& w,
	IteratorState (*in_pItCreate)(std::vector<unsigned int>*),
	DoubleIterator<unsigned int, IteratorState>& itInterface
	)
{
	std::vector<unsigned int> result;
	IteratorState itState = (*in_pItCreate)(&v);
	runIteratorForward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
	itState = (*in_pItCreate)(&v);
	reverseVector(&w);
	runIteratorBackward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
}

#endif
