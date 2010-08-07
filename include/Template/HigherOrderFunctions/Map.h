#ifndef _HigherOrderFunctions_h
#define _HigherOrderFunctions_h

#include <cassert>
#include "Util/Iterator.h"

template <typename State, typename Type> void map(SingleIterator* in_pIt, 
												  void (*in_pfFunc)(Type*))
{
	while (!(*(in_pIt->mpfEnd))())
	{
		Type* pCurrentValue = (Type*) (*(in_pIt->mpfGet))();
		(*in_pFunc)(*pCurrentValue);

		(*(in_pIt->mpfNext))(in_pIt->mState);
	}
}

#endif
