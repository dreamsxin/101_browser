#ifndef _PositionIteratorState_h
#define _PositionIteratorState_h

#include <vector>
// for size_t
#include "MiniStdlib/cstddef.h"

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

template <typename IteratorState> void* positionIterator_get(void* in_pIts)
{
	const IteratorState* pIts = (const IteratorState*) in_pIts;

	if (pIts->mCurrentPosition < pIts->mpVector->size())
		return &(pIts->mpVector->at(pIts->mCurrentPosition));
	else
		return NULL;
}

#endif
