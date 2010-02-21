#ifndef _TriangleStripBorderIterator_h
#define _TriangleStripBorderIterator_h

#include "BasicDataStructures/Iterator.h"
#include <vector>
#include <cassert>

template <typename Type> struct TriangleStripBorderIteratorState
{
	std::vector<Type>* mpVector;
	size_t mCurrentPosition;
};

template <typename Type> struct TriangleStripBorderConstIteratorState
{
	const std::vector<Type>* mpVector;
	size_t mCurrentPosition;
};

template <typename Type> TriangleStripBorderIteratorState<Type> 
triangleStripBorderIteratorState_create(std::vector<Type>* in_pVector)
{
	TriangleStripBorderIteratorState<Type> out_state = {in_pVector, 0};
	return out_state;
}

template <typename Type> TriangleStripBorderConstIteratorState<Type> 
triangleStripBorderConstIteratorState_create(const std::vector<Type>* in_pVector)
{
	TriangleStripBorderConstIteratorState<Type> out_state = {in_pVector, 0};
	return out_state;
}

template <typename Type, typename IteratorState> Type* triangleStripBorderIterator_get(
	const IteratorState* in_pIts)
{
	if (in_pIts->mCurrentPosition < in_pIts->mpVector->size())
		return &(in_pIts->mpVector->at(in_pIts->mCurrentPosition));
	else
		return NULL;
}

template <typename Type, typename IteratorState> IterateResult triangleStripBorderIterator_next(
	IteratorState* in_pIts)
{
	if (in_pIts->mCurrentPosition==0)
	{
		switch (in_pIts->mpVector->size())
		{
		case 0:
			return IterateResultEndToEnd;
		case 1:
			return IterateResultEndToStart;
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
					return IterateResultEndToStart;
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
				return IterateResultEndToStart;
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
			return IterateResultEndToEnd;
		case 1:
			return IterateResultEndToStart;
		case 2:
			in_pIts->mCurrentPosition = 1;
			return IterateResultEndToStart;
		default:
			in_pIts->mCurrentPosition = 2;
			return IterateResultEndToStart;
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

template <typename Type> DoubleIterator<Type, TriangleStripBorderIteratorState<Type> >
triangleStripBorderIterator_create()
{
	DoubleIterator<Type, TriangleStripBorderIteratorState<Type>> out_iter = 
	{
		&triangleStripBorderIterator_get,
		&triangleStripBorderIterator_next<Type, TriangleStripBorderIteratorState<Type> >,
		&triangleStripBorderIterator_prev<Type, TriangleStripBorderIteratorState<Type> >
	};

	return out_iter;
}

template <typename Type> DoubleIterator<const Type, TriangleStripBorderConstIteratorState<Type> >
triangleStripBorderConstIterator_create()
{
	DoubleIterator<const Type, TriangleStripBorderConstIteratorState<Type> > out_iter = 
	{
		&triangleStripBorderIterator_get,
		&triangleStripBorderIterator_next<Type, TriangleStripBorderConstIteratorState<Type> >,
		&triangleStripBorderIterator_prev<Type, TriangleStripBorderConstIteratorState<Type> >
	};

	return out_iter;
}

#endif
