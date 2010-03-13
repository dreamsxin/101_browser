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

#endif
