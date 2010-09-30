#include <assert.h>
#include <stdlib.h> // for NULL
#include "Algorithm/IterateWhile.h"

bool iterateWhile(SingleIterator in_iterator, void* in_iteratorState, void* in_pUserdata,
				  bool (*in_pConditionFunction)(const void* in_pCurrentElement, const void* in_pUserdata))
{
	void* pCurrentElement = (*in_iterator.mpfGet)(in_iteratorState);

	if (pCurrentElement == NULL)
		return false;

	while ((*in_pConditionFunction)(pCurrentElement, in_pUserdata))
	{
		IterateResult itResult = (*in_iterator.mpfIterate)(in_iteratorState);

		if (itResult != IterateResultOK)
			return false;

		pCurrentElement = (*in_iterator.mpfGet)(in_iteratorState);

		assert(pCurrentElement != NULL);
	}

	return true;
}
