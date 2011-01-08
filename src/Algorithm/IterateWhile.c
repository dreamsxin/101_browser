/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
