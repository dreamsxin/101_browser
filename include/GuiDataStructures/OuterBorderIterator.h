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
	assert(in_pVector->size()>=4 || in_pVector->size()==0);
	assert(in_pVector->size()%2==0);

	OuterBorderIterator<Type>::IteratorState out_state = {in_pVector, 1};
	return out_state;
}

template <typename Type> typename OuterBorderIterator<Type>::ConstIteratorState
outerBorderConstIteratorState_create(const std::vector<Type>* in_pVector)
{
	assert(in_pVector->size()>=4 || in_pVector->size()==0);
	assert(in_pVector->size()%2==0);

	OuterBorderIterator<Type>::ConstIteratorState out_state = {in_pVector, 1};
	return out_state;
}

template <typename Type, typename IteratorState> IterateResult outerBorderIterator_next(
	IteratorState* in_pIts)
{
	assert(in_pIts->mpVector->size()>=4 || in_pIts->mpVector->size()==0);
	assert(in_pIts->mpVector->size()%2==0);
	assert(in_pIts->mCurrentPosition%2==1);
	in_pIts->mCurrentPosition+=2;
	assert(in_pIts->mCurrentPosition%2==1);

	if (in_pIts->mCurrentPosition+2>=in_pIts->mpVector->size())
	{
		in_pIts->mCurrentPosition = 1;

		if (in_pIts->mpVector->size()==0)
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

template <typename Type, typename IteratorState> IterateResult outerBorderIterator_prev(
	IteratorState* in_pIts)
{
	assert(in_pIts->mpVector->size()>=4 || in_pIts->mpVector->size()==0);
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
		if (in_pIts->mpVector->size() == 0)
		{
			assert(in_pIts->mCurrentPosition == 1);
			return IterateResultToInvalidState;
		}
		else
		{
			in_pIts->mCurrentPosition = in_pIts->mpVector->size()-3;
			return IterateResultOverBoundary;
		}
	}
}

template <typename Type> DoubleIterator<Type, typename OuterBorderIterator<Type>::IteratorState>
outerBorderIterator_create()
{
	DoubleIterator<Type, OuterBorderIterator<Type>::IteratorState> out_iter = 
	{
		&positionIterator_get,
		&outerBorderIterator_next<Type, OuterBorderIterator<Type>::IteratorState>,
		&outerBorderIterator_prev<Type, OuterBorderIterator<Type>::IteratorState>
	};

	return out_iter;
}

template <typename Type> DoubleIterator<const Type, typename OuterBorderIterator<Type>::ConstIteratorState>
outerBorderConstIterator_create()
{
	DoubleIterator<const Type, OuterBorderIterator<Type>::ConstIteratorState> out_iter = 
	{
		&positionIterator_get,
		&outerBorderIterator_next<Type, OuterBorderIterator<Type>::ConstIteratorState>,
		&outerBorderIterator_prev<Type, OuterBorderIterator<Type>::ConstIteratorState>
	};

	return out_iter;
}

template <typename Type> DoubleIteratorInstance<const Type, typename OuterBorderIterator<Type>::ConstIteratorState>
outerBorderConstIteratorInstance_create(const std::vector<Type>* in_pVector)
{
	DoubleIteratorInstance<const Type, OuterBorderIterator<Type>::ConstIteratorState>
		inst = {outerBorderConstIteratorState_create(in_pVector), outerBorderConstIterator_create<Type>()};

	return inst;
}

#endif
