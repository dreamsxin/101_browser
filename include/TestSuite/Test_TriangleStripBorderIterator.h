#ifndef _Test_TriangleStripBorderIterator_h
#define _Test_TriangleStripBorderIterator_h

#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

template 
<typename UnsignedIntType,
/*
 * this is only for declaration of in_pfIteratorState_create
 * it has either to be 
 * const std::vector<unsigned int>
 * or
 * std::vector<unsigned int>
 */
typename VectorType, 
typename IteratorStateType>
void testTriangleStripBorderIterator
(
 IteratorStateType (*in_pfIteratorState_create)(VectorType*),
 DoubleIterator<UnsignedIntType, IteratorStateType> (*in_pfIterator_create)()
)
{
	std::vector<UnsignedIntType> v, w, result;
	DoubleIterator<UnsignedIntType, IteratorStateType> 
		itInterface = (*in_pfIterator_create)();

	// Testing correct behaviour for small datasets

	v=std::vector<UnsignedIntType>();
	w=std::vector<UnsignedIntType>();
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	w=std::vector<UnsignedIntType>();
	w.push_back(0);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	w=std::vector<UnsignedIntType>();
	w.push_back(0);
	w.push_back(1);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	w=std::vector<UnsignedIntType>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(2);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	// Now the next two non-trivial cases

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	w=std::vector<UnsignedIntType>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(3);
	w.push_back(2);
	testIterator(v, w, in_pfIteratorState_create, itInterface);

	v=std::vector<UnsignedIntType>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	w=std::vector<UnsignedIntType>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(3);
	w.push_back(4);
	w.push_back(2);
	testIterator(v, w, in_pfIteratorState_create, itInterface);
}

#endif
