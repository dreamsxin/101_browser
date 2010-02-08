#ifndef _HigherOrderFunctions_h
#define _HigherOrderFunctions_h

#include <cassert>
#include "BasicDataStructures/Iterator.h"

template <typename State, typename Type> void map(Iterator<State, Type>* in_pIt, 
											 void (*in_pfFunc)(Type*))
{
	assert(in_pIt->mfpEnd);
	assert(in_pIt->mfpGet);
	assert(in_pIt->mfpNext);

	while (!(*(in_pIt->mfpEnd))())
	{
		Type* pCurrentValue = (*(in_pIt->mfpGet))();
		(*in_pFunc)(*pCurrentValue);

		(*(in_pIt->mfpNext))(in_pIt->mState);
	}
}

#endif
