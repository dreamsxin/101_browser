#ifndef _OuterBorderIterator_h
#define _OuterBorderIterator_h

#include <cassert>
#include "Util/Iterator.h"
#include "Util/IteratorInstance.hpp"
#include "GuiDataStructures/PositionIteratorState.h"

template <typename Type> class OuterBorderIterator
{
public:
	typedef PositionIteratorState<Type> IteratorState;
};

template <typename Type> typename OuterBorderIterator<Type>::IteratorState
outerBorderIteratorState_create(std::vector<Type>* in_pVector)
{
	assert(in_pVector->size()>=4 || in_pVector->size()==0);
	assert(in_pVector->size()%2==0);

	typename OuterBorderIterator<Type>::IteratorState out_state = {in_pVector, 1};
	return out_state;
}

template <typename Type> typename OuterBorderIterator<Type>::ConstIteratorState
outerBorderConstIteratorState_create(const std::vector<Type>* in_pVector)
{
	assert(in_pVector->size()>=4 || in_pVector->size()==0);
	assert(in_pVector->size()%2==0);

	typename OuterBorderIterator<Type>::ConstIteratorState out_state = {in_pVector, 1};
	return out_state;
}

template <typename IteratorState> IterateResult outerBorderIterator_next(void* in_pIts)
{
	IteratorState* pIts = (IteratorState*) in_pIts;

	assert(pIts->mpVector->size()>=4 || pIts->mpVector->size()==0);
	assert(pIts->mpVector->size()%2==0);
	assert(pIts->mCurrentPosition%2==1);
	pIts->mCurrentPosition+=2;
	assert(pIts->mCurrentPosition%2==1);

	if (pIts->mCurrentPosition+2 >= pIts->mpVector->size())
	{
		pIts->mCurrentPosition = 1;

		if (pIts->mpVector->size()==0)
		{
			return IterateResultToInvalidState;
		}
		else
		{
			return IterateResultOverBoundary;
		}
	}
	else
	{
		return IterateResultOK;
	}
}

template <typename IteratorState> IterateResult outerBorderIterator_prev(void* in_pIts)
{
	IteratorState* pIts = (IteratorState*) in_pIts;

	assert(pIts->mpVector->size() >= 4 || pIts->mpVector->size() == 0);
	assert(pIts->mpVector->size()%2 == 0);
	assert(pIts->mCurrentPosition%2 == 1);

	if (pIts->mCurrentPosition != 1)
	{
		assert(pIts->mCurrentPosition >= 3);
		pIts->mCurrentPosition -= 2;
		assert(pIts->mCurrentPosition%2 == 1);
		return IterateResultOK;
	}
	else
	{
		if (pIts->mpVector->size() == 0)
		{
			assert(pIts->mCurrentPosition == 1);
			return IterateResultToInvalidState;
		}
		else
		{
			pIts->mCurrentPosition = pIts->mpVector->size()-3;
			return IterateResultOverBoundary;
		}
	}
}

template <typename Type> DoubleIterator outerBorderIterator_create()
{
	DoubleIterator out_iter = 
	{
		&positionIterator_get<typename OuterBorderIterator<Type>::IteratorState>,
		&outerBorderIterator_next<typename OuterBorderIterator<Type>::IteratorState>,
		&outerBorderIterator_prev<typename OuterBorderIterator<Type>::IteratorState>
	};

	return out_iter;
}

template <typename Type> DoubleIteratorInstance<typename OuterBorderIterator<Type>::IteratorState>
outerBorderIteratorInstance_create(std::vector<Type>* in_pVector)
{
	DoubleIteratorInstance<typename OuterBorderIterator<Type>::IteratorState>
		inst = {outerBorderIteratorState_create(in_pVector), outerBorderIterator_create<Type>()};

	return inst;
}

#endif
