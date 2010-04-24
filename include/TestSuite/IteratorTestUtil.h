#ifndef _IteratorTestUtil_h
#define _IteratorTestUtil_h

#include <vector>
#include "BasicDataStructures/Iterator.h"
#include "TestSuite/TestSuite.h"

template <typename IteratorState, typename UnsignedIntType> 
void runIteratorForward(IteratorState* in_pItState, 
						DoubleIterator<UnsignedIntType, IteratorState> in_interface,
						std::vector<UnsignedIntType>* in_out_pResult)
{
	*in_out_pResult=std::vector<UnsignedIntType>();

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

template <typename IteratorState, typename UnsignedIntType> 
void runIteratorBackward(IteratorState* in_pItState, 
						 DoubleIterator<UnsignedIntType, IteratorState> in_interface,
						 std::vector<UnsignedIntType>* in_out_pResult)
{
	*in_out_pResult=std::vector<UnsignedIntType>();

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

template <typename UnsignedIntType>
bool compareVectors(
					const std::vector<UnsignedIntType>* in_pV, 
					const std::vector<UnsignedIntType>* in_pW)
{
	test(in_pV->size()==in_pW->size());

	std::vector<UnsignedIntType>::const_iterator i, j;

	for (i=in_pV->begin(), j=in_pW->begin(); i!=in_pV->end() && j!=in_pW->end(); i++, j++)
	{
		if (*i!=*j)
			return false;
	}

	return true;
}

template <typename UnsignedIntType>
void reverseVector(std::vector<UnsignedIntType>* in_pV)
{
	for (size_t i=0; i<in_pV->size()/2; i++)
	{
		std::swap(in_pV->at(i), in_pV->at(in_pV->size()-1-i));
	}
}

template <typename UnsignedIntType, 
typename VectorType, 
typename IteratorState> 
void testIterator(
	std::vector<UnsignedIntType>& v, 
	std::vector<UnsignedIntType>& w,
	IteratorState (*in_pItCreate)(VectorType*),
	DoubleIterator<UnsignedIntType, IteratorState>& itInterface
	)
{
	std::vector<UnsignedIntType> result;
	IteratorState itState = (*in_pItCreate)(&v);
	runIteratorForward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
	itState = (*in_pItCreate)(&v);
	reverseVector(&w);
	runIteratorBackward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
}

#endif
