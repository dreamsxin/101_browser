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

#ifndef _SingleChain_h
#define _SingleChain_h

#include <cassert>

template<typename T> struct SingleChain
{
	SingleChain<T>* next;
	T node;
};

template<typename T> void appendSingleChain(SingleChain<T>* in_pNewHeadSingleChain,
											SingleChain<T>** in_ppFutureTailSingleChain)
{
	assert(in_pNewHeadSingleChain->next==NULL);

	in_pNewHeadSingleChain->next=*in_ppFutureTailSingleChain;
	*in_ppFutureTailSingleChain = in_pNewHeadSingleChain;
}

#endif
