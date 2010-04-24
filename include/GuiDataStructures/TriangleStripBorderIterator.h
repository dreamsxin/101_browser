#ifndef _TriangleStripBorderIterator_h
#define _TriangleStripBorderIterator_h

#include <cassert>
#include "BasicDataStructures/Iterator.h"
#include "GuiDataStructures/PositionIteratorState.h"

template <typename Type> class TriangleStripBorderIterator
{
public:
	typedef PositionIteratorState<Type> IteratorState;
	typedef PositionConstIteratorState<Type> ConstIteratorState;
};

template <typename Type> typename TriangleStripBorderIterator<Type>::IteratorState
triangleStripBorderIteratorState_create(std::vector<Type>* in_pVector)
{
	typename TriangleStripBorderIterator<Type>::IteratorState out_state = {in_pVector, 0};
	return out_state;
}

template <typename Type> typename TriangleStripBorderIterator<Type>::ConstIteratorState
triangleStripBorderConstIteratorState_create(const std::vector<Type>* in_pVector)
{
	typename TriangleStripBorderIterator<Type>::ConstIteratorState out_state = {in_pVector, 0};
	return out_state;
}

template <typename Type, typename IteratorState> IterateResult triangleStripBorderIterator_next(
	IteratorState* in_pIts)
{
	if (in_pIts->mCurrentPosition==0)
	{
		switch (in_pIts->mpVector->size())
		{
		case 0:
			return IterateResultToInvalidState;
		case 1:
			return IterateResultOverBoundary;
		default:
			in_pIts->mCurrentPosition++;
			return IterateResultOK;
		}
	}
	else
	{
		if (in_pIts->mCurrentPosition%2==1)
		{
			if (in_pIts->mCurrentPosition+2<in_pIts->mpVector->size())
			{
				in_pIts->mCurrentPosition+=2;
				return IterateResultOK;
			}
			else if (in_pIts->mCurrentPosition+1<in_pIts->mpVector->size())
			{
				in_pIts->mCurrentPosition+=1;
				return IterateResultOK;
			}
			else
			{
				// Since the other conditions in the if/else if part are
				// wrong and we have in_pIts->mCurrentPosition<in_pIts->mpVector->size()
				// we get in_pIts->mCurrentPosition == in_pIts->mpVector->size()-1
				assert(in_pIts->mCurrentPosition == in_pIts->mpVector->size()-1);

				in_pIts->mCurrentPosition-=1;
				if (in_pIts->mCurrentPosition==0)
					return IterateResultOverBoundary;
				else
					return IterateResultOK;
			}
		}
		else
		{
			// As we have checked above we have in_pIts->mCurrentPosition!=0. 
			// So we have in_pIts->mCurrentPosition>=2
			assert(in_pIts->mCurrentPosition>=2);

			in_pIts->mCurrentPosition-=2;

			if (in_pIts->mCurrentPosition==0)
				return IterateResultOverBoundary;
			else
				return IterateResultOK;
		}
	}
}

template <typename Type, typename IteratorState> IterateResult triangleStripBorderIterator_prev(
	IteratorState* in_pIts)
{
	if (in_pIts->mCurrentPosition==0)
	{
		switch (in_pIts->mpVector->size())
		{
		case 0:
			return IterateResultToInvalidState;
		case 1:
			return IterateResultOverBoundary;
		case 2:
			in_pIts->mCurrentPosition = 1;
			return IterateResultOverBoundary;
		default:
			in_pIts->mCurrentPosition = 2;
			return IterateResultOverBoundary;
		}
	}
	else
	{
		if (in_pIts->mCurrentPosition%2==0)
		{
			if (in_pIts->mCurrentPosition+2<in_pIts->mpVector->size())
				in_pIts->mCurrentPosition+=2;
			else if (in_pIts->mCurrentPosition+1<in_pIts->mpVector->size())
				in_pIts->mCurrentPosition+=1;
			else
			{
				// Since the other conditions in the if/else if part are
				// wrong and we have in_pIts->mCurrentPosition<in_pIts->mpVector->size()
				// we get in_pIts->mCurrentPosition == in_pIts->mpVector->size()-1
				assert(in_pIts->mCurrentPosition == in_pIts->mpVector->size()-1);

				// As we have checked above we have in_pIts->mCurrentPosition!=0. 
				// So we have in_pIts->mCurrentPosition>=2
				assert(in_pIts->mCurrentPosition>=2);

				in_pIts->mCurrentPosition-=1;
				
			}

			return IterateResultOK;
		}
		else
		{
			if (in_pIts->mCurrentPosition==1)
				in_pIts->mCurrentPosition=0;
			else
			{
				assert(in_pIts->mCurrentPosition>=3);
				in_pIts->mCurrentPosition-=2;
			}

			return IterateResultOK;
		}
	}
}

template <typename Type> DoubleIterator<Type, typename TriangleStripBorderIterator<Type>::IteratorState>
triangleStripBorderIterator_create()
{
	DoubleIterator<Type, typename TriangleStripBorderIterator<Type>::IteratorState> out_iter = 
	{
		&positionIterator_get,
		&triangleStripBorderIterator_next<Type, typename TriangleStripBorderIterator<Type>::IteratorState>,
		&triangleStripBorderIterator_prev<Type, typename TriangleStripBorderIterator<Type>::IteratorState>
	};

	return out_iter;
}

template <typename Type> DoubleIterator<Type, typename TriangleStripBorderIterator<Type>::ConstIteratorState>
triangleStripBorderConstIterator_create()
{
	DoubleIterator<Type, typename TriangleStripBorderIterator<Type>::ConstIteratorState> out_iter = 
	{
		&positionIterator_get,
		&triangleStripBorderIterator_next<Type, typename TriangleStripBorderIterator<Type>::ConstIteratorState>,
		&triangleStripBorderIterator_prev<Type, typename TriangleStripBorderIterator<Type>::ConstIteratorState>
	};

	return out_iter;
}

template <typename Type> DoubleIteratorInstance<const Type, typename TriangleStripBorderIterator<Type>::IteratorState>
triangleStripBorderIteratorInstance_create(const std::vector<Type>* in_pVector)
{
	DoubleIteratorInstance<Type, typename TriangleStripBorderIterator<Type>::IteratorState>
		inst = 
	{
		triangleStripBorderIteratorState_create(in_pVector), 
		triangleStripBorderIterator_create<Type>()
	};

	return inst;
}

template <typename Type> DoubleIteratorInstance<const Type, typename TriangleStripBorderIterator<const Type>::ConstIteratorState>
triangleStripBorderConstIteratorInstance_create(const std::vector<Type>* in_pVector)
{
	DoubleIteratorInstance<const Type, typename TriangleStripBorderIterator<const Type>::ConstIteratorState>
		inst = 
	{
		triangleStripBorderConstIteratorState_create(in_pVector), 
		triangleStripBorderConstIterator_create<Type>()
	};

	return inst;
}

#endif
