#ifndef _PositionIteratorState_h
#define _PositionIteratorState_h

#include <vector>

template <typename Type> struct PositionIteratorState
{
	std::vector<Type>* mpVector;
	size_t mCurrentPosition;
};

template <typename Type> struct PositionConstIteratorState
{
	const std::vector<Type>* mpVector;
	size_t mCurrentPosition;
};

template <typename Type, typename IteratorState> Type* positionIterator_get(
	const IteratorState* in_pIts)
{
	if (in_pIts->mCurrentPosition < in_pIts->mpVector->size())
		return &(in_pIts->mpVector->at(in_pIts->mCurrentPosition));
	else
		return NULL;
}

#endif
