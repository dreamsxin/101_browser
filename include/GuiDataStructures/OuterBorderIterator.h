#ifndef _OuterBorderIterator_h
#define _OuterBorderIterator_h

#include <cassert>
#include "BasicDataStructures/Iterator.h"
#include "GuiDataStructures/PositionIteratorState.h"

template <typename Type> class OuterBorderIterator
{
public:
	typedef PositionIteratorState<Type> IteratorState;
	typedef PositionConstIteratorState<Type> ConstIteratorState;
};

template <typename Type> typename OuterBorderIterator<Type>::IteratorState
outerBorderIteratorState_create(std::vector<Type>* in_pVector)
{
	assert(in_pVector->size()>=2);
	assert(in_pVector->size()%2==0);

	OuterBorderIterator<Type>::IteratorState out_state = {in_pVector, 1};
	return out_state;
}

template <typename Type> typename OuterBorderIterator<Type>::ConstIteratorState
outerBorderConstIteratorState_create(const std::vector<Type>* in_pVector)
{
	assert(in_pVector->size()>=2);
	assert(in_pVector->size()%2==0);

	OuterBorderIterator<Type>::ConstIteratorState out_state = {in_pVector, 1};
	return out_state;
}

template <typename Type, typename IteratorState> IterateResult outerBorderIterator_next(
	IteratorState* in_pIts)
{
	assert(in_pIts->mpVector->size()%2==0);
	assert(in_pIts->mCurrentPosition%2==1);
	in_pIts->mCurrentPosition+=2;
	assert(in_pIts->mCurrentPosition%2==1);

	if (in_pIts->mCurrentPosition>=in_pIts->mpVector->size())
	{
		in_pIts->mCurrentPosition = 1;
		return IterateResultOverBoundary;
	}
	else
	{
		return IterateResultOK;
	}
}

template <typename Type, typename IteratorState> IterateResult outerBorderIterator_prev(
	IteratorState* in_pIts)
{
	assert(in_pIts->mpVector->size()%2==0);
	assert(in_pIts->mCurrentPosition%2==1);

	if (in_pIts->mCurrentPosition != 1)
	{
		assert(in_pIts->mCurrentPosition>=3);
		in_pIts->mCurrentPosition-=2;
		assert(in_pIts->mCurrentPosition%2==1);
		return IterateResultOK;
	}
	else
	{
		in_pIts->mCurrentPosition = in_pIts->mpVector->size()-1;
		return IterateResultOverBoundary;
	}
}

#endif
