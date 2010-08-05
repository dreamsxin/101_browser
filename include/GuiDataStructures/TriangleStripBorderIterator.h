#ifndef _TriangleStripBorderIterator_h
#define _TriangleStripBorderIterator_h

#include <cassert>
#include "Util/Iterator.h"
#include "Util/IteratorInstance.hpp"
#include "GuiDataStructures/PositionIteratorState.h"

template <typename Type> class TriangleStripBorderIterator
{
public:
	typedef PositionIteratorState<Type> IteratorState;
};

template <typename Type> typename TriangleStripBorderIterator<Type>::IteratorState
triangleStripBorderIteratorState_create(std::vector<Type>* in_pVector)
{
	typename TriangleStripBorderIterator<Type>::IteratorState out_state = {in_pVector, 0};
	return out_state;
}

template <typename IteratorState> IterateResult triangleStripBorderIterator_next(void* in_pIts)
{
	IteratorState* pIts = (IteratorState*) in_pIts;

	if (pIts->mCurrentPosition==0)
	{
		switch (pIts->mpVector->size())
		{
		case 0:
			return IterateResultToInvalidState;
		case 1:
			return IterateResultOverBoundary;
		default:
			pIts->mCurrentPosition++;
			return IterateResultOK;
		}
	}
	else
	{
		if (pIts->mCurrentPosition%2==1)
		{
			if (pIts->mCurrentPosition+2 < pIts->mpVector->size())
			{
				pIts->mCurrentPosition+=2;
				return IterateResultOK;
			}
			else if (pIts->mCurrentPosition+1 < pIts->mpVector->size())
			{
				pIts->mCurrentPosition+=1;
				return IterateResultOK;
			}
			else
			{
				// Since the other conditions in the if/else if part are
				// wrong and we have in_pIts->mCurrentPosition<in_pIts->mpVector->size()
				// we get in_pIts->mCurrentPosition == in_pIts->mpVector->size()-1
				assert(pIts->mCurrentPosition == pIts->mpVector->size()-1);

				pIts->mCurrentPosition-=1;
				if (pIts->mCurrentPosition==0)
					return IterateResultOverBoundary;
				else
					return IterateResultOK;
			}
		}
		else
		{
			// As we have checked above we have in_pIts->mCurrentPosition!=0. 
			// So we have in_pIts->mCurrentPosition>=2
			assert(pIts->mCurrentPosition>=2);

			pIts->mCurrentPosition-=2;

			if (pIts->mCurrentPosition==0)
				return IterateResultOverBoundary;
			else
				return IterateResultOK;
		}
	}
}

template <typename IteratorState> IterateResult triangleStripBorderIterator_prev(void* in_pIts)
{
	IteratorState* pIts = (IteratorState*) in_pIts;

	if (pIts->mCurrentPosition==0)
	{
		switch (pIts->mpVector->size())
		{
		case 0:
			return IterateResultToInvalidState;
		case 1:
			return IterateResultOverBoundary;
		case 2:
			pIts->mCurrentPosition = 1;
			return IterateResultOverBoundary;
		default:
			pIts->mCurrentPosition = 2;
			return IterateResultOverBoundary;
		}
	}
	else
	{
		if (pIts->mCurrentPosition%2==0)
		{
			if (pIts->mCurrentPosition+2 < pIts->mpVector->size())
				pIts->mCurrentPosition+=2;
			else if (pIts->mCurrentPosition+1 < pIts->mpVector->size())
				pIts->mCurrentPosition+=1;
			else
			{
				// Since the other conditions in the if/else if part are
				// wrong and we have in_pIts->mCurrentPosition<in_pIts->mpVector->size()
				// we get in_pIts->mCurrentPosition == in_pIts->mpVector->size()-1
				assert(pIts->mCurrentPosition == pIts->mpVector->size()-1);

				// As we have checked above we have in_pIts->mCurrentPosition!=0. 
				// So we have in_pIts->mCurrentPosition>=2
				assert(pIts->mCurrentPosition>=2);

				pIts->mCurrentPosition-=1;
			}

			return IterateResultOK;
		}
		else
		{
			if (pIts->mCurrentPosition==1)
				pIts->mCurrentPosition=0;
			else
			{
				assert(pIts->mCurrentPosition>=3);
				pIts->mCurrentPosition-=2;
			}

			return IterateResultOK;
		}
	}
}

template <typename Type> DoubleIterator triangleStripBorderIterator_create()
{
	DoubleIterator out_iter = 
	{
		&positionIterator_get<typename TriangleStripBorderIterator<Type>::IteratorState>,
		&triangleStripBorderIterator_next<typename TriangleStripBorderIterator<Type>::IteratorState>,
		&triangleStripBorderIterator_prev<typename TriangleStripBorderIterator<Type>::IteratorState>
	};

	return out_iter;
}

template <typename Type> DoubleIteratorInstance<typename TriangleStripBorderIterator<Type>::IteratorState>
triangleStripBorderIteratorInstance_create(std::vector<Type>* in_pVector)
{
	DoubleIteratorInstance<typename TriangleStripBorderIterator<Type>::IteratorState>
		inst = {triangleStripBorderIteratorState_create(in_pVector), triangleStripBorderIterator_create<Type>()};

	return inst;
}

#endif
