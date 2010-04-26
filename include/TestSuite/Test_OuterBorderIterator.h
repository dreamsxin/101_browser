#ifndef _Test_OuterBorderIterator_h
#define _Test_OuterBorderIterator_h

#include "GuiDataStructures/OuterBorderIterator.h"
#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

template <
typename UnsignedIntType, 
/*
 * this is only for declaration of in_pfIteratorState_create
 * it has either to be 
 * const std::vector<unsigned int>
 * or
 * std::vector<unsigned int>
 */
typename VectorType, 
typename IteratorStateType, 
typename IteratorType> 
void testOuterBorderIterator(
	IteratorStateType (*in_pfIteratorState_create)(VectorType* in_pVector),
	IteratorType (*in_pfIterator_create)()
	)
{
	std::vector<UnsignedIntType> v, w, result;
	IteratorStateType itState;
	
	IteratorType itInterface = (*in_pfIterator_create)();

	v=std::vector<UnsignedIntType>();
	w=std::vector<UnsignedIntType>();
	itState = (*in_pfIteratorState_create)(&v);
	runIteratorForward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
	itState = (*in_pfIteratorState_create)(&v);
	reverseVector(&w);
	runIteratorBackward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

	// Skipping case of length 2 because it makes no sense 
	// because it is expected that the last 2 vector components
	// repeat the first 2 and so there are at least 4 vector
	// components

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	w=std::vector<UnsignedIntType>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	w=std::vector<UnsignedIntType>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	w=std::vector<UnsignedIntType>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	w.push_back(5);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	v.push_back(8);
	v.push_back(9);
	w=std::vector<UnsignedIntType>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	w.push_back(5);
	w.push_back(7);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	v.push_back(8);
	v.push_back(9);
	v.push_back(10);
	v.push_back(11);
	w=std::vector<UnsignedIntType>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	w.push_back(5);
	w.push_back(7);
	w.push_back(9);
	testIterator(v, w, in_pfIteratorState_create, itInterface);
}

#endif
