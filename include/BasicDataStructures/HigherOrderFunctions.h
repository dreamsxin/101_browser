#ifndef _HigherOrderFunctions_h
#define _HigherOrderFunctions_h

#include <cassert>
#include "BasicDataStructures/Iterator.h"

template <typename State, typename Type> void map(Iterator<State, Type>* in_pIt, 
											 void (*in_pfFunc)(Type*))
{
	assert(in_pIt->mpfEnd);
	assert(in_pIt->mpfGet);
	assert(in_pIt->mpfNext);

	while (!(*(in_pIt->mpfEnd))())
	{
		Type* pCurrentValue = (*(in_pIt->mpfGet))();
		(*in_pFunc)(*pCurrentValue);

		(*(in_pIt->mpfNext))(in_pIt->mState);
	}
}

#endif
