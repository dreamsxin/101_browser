#ifndef _HigherOrderFunctions_h
#define _HigherOrderFunctions_h

#include <cassert>
#include "BasicDataStructures/Iterator.h"

template <typename State, typename Type> void map(SingleIterator<State, Type>* in_pIt, 
												  void (*in_pfFunc)(Type*))
{
	while (!(*(in_pIt->mpfEnd))())
	{
		Type* pCurrentValue = (*(in_pIt->mpfGet))();
		(*in_pFunc)(*pCurrentValue);

		(*(in_pIt->mpfNext))(in_pIt->mState);
	}
}

#endif
