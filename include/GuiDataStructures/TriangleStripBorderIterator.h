#ifndef _TriangleStripBorderIterator_h
#define _TriangleStripBorderIterator_h

#include "BasicDataStructures/Iterator.h"
#include <vector>
#include <cassert>

template <typename Type> struct TriangleStripBorderIteratorState
{
	std::vector<Type>* mVector;
	size_t mCurrentPosition;
	bool mAtEnd;
};

template <typename Type> struct TriangleStripBorderConstIteratorState
{
	const std::vector<Type>* mVector;
	size_t mCurrentPosition;
	bool mAtEnd;
};

template <typename Type> TriangleStripBorderIteratorState<Type> 
triangleStripBorderIteratorState_create(std::vector<Type>* in_pVector)
{
	TriangleStripBorderIteratorState<Type> out_state = {in_pVector, 0, in_pVector->empty()};
	return out_state;
}

template <typename Type> TriangleStripBorderConstIteratorState<Type> 
triangleStripBorderConstIteratorState_create(const std::vector<Type>* in_pVector)
{
	TriangleStripBorderConstIteratorState<Type> out_state = {in_pVector, 0, in_pVector->empty()};
	return out_state;
}

template <typename Type, typename IteratorState> bool triangleStripBorderIterator_end(
	const IteratorState* in_pIts)
{
	return in_pIts->mAtEnd;
}

template <typename Type, typename IteratorState> Type* triangleStripBorderIterator_get(
	const IteratorState* in_pIts)
{
	return &(in_pIts->mVector->at(in_pIts->mCurrentPosition));
}

template <typename Type, typename IteratorState> void triangleStripBorderIterator_next(
	IteratorState* in_pIts)
{
	// If it *mVector is empty mAtEnd is always set
	// You should NEVER call this function if the iterator is at the end
	assert(!in_pIts->mAtEnd);

	if (in_pIts->mCurrentPosition==0)
	{
		if (in_pIts->mVector->size()>1)
		{
			in_pIts->mCurrentPosition++;
		}
		else
		{
			in_pIts->mAtEnd = true;
		}
	}
	else
	{
		if (in_pIts->mCurrentPosition%2==1)
		{
			if (in_pIts->mCurrentPosition+2<in_pIts->mVector->size())
			{
				in_pIts->mCurrentPosition+=2;
			}
			else if (in_pIts->mCurrentPosition+1<in_pIts->mVector->size())
			{
				in_pIts->mCurrentPosition+=1;
			}
			else
			{
				in_pIts->mCurrentPosition-=1;
				if (in_pIts->mCurrentPosition==0)
					in_pIts->mAtEnd = true;
			}
		}
		else
		{
			// As we have checked above we have in_pIts->mCurrentPosition!=0. 
			// So we have in_pIts->mCurrentPosition>=2
			in_pIts->mCurrentPosition-=2;
			if (in_pIts->mCurrentPosition==0)
				in_pIts->mAtEnd = true;
		}
	}

}

template <typename Type> Iterator<Type, TriangleStripBorderIteratorState<Type> >
triangleStripBorderIterator_create()
{
	Iterator<Type, TriangleStripBorderIteratorState<Type>> out_iter = 
	{
		&triangleStripBorderIterator_end<Type, TriangleStripBorderIteratorState<Type> >,
		&triangleStripBorderIterator_get,
		&triangleStripBorderIterator_next<Type, TriangleStripBorderIteratorState<Type> >
	};

	return out_iter;
}

template <typename Type> Iterator<const Type, TriangleStripBorderConstIteratorState<Type>>
triangleStripBorderConstIterator_create()
{
	Iterator<const Type, TriangleStripBorderConstIteratorState<Type> > out_iter = 
	{
		&triangleStripBorderIterator_end<Type, TriangleStripBorderConstIteratorState<Type> >,
		&triangleStripBorderIterator_get,
		&triangleStripBorderIterator_next<Type, TriangleStripBorderConstIteratorState<Type> >
	};

	return out_iter;
}

#endif
