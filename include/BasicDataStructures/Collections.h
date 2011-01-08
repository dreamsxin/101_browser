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

#ifndef _Collections_h
#define _Collections_h

template <typename Type, typename CollectionState> struct Collection
{

};

template <typename CollectionState> struct Clearable
{
	void (*mpfClear)(CollectionState* in_pCollectionState);
};

template <typename Type, typename CollectionState> struct Set
{
	bool (*mpfContains)(CollectionState* in_pCollectionState, const Type* in_pcItem);
};

template <typename Type, typename CollectionState, typename IteratorState> struct List
{
	void (*mpfAppendToEnd)(CollectionState* state, Type t);
};

#endif
