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

#include "GuiDataStructures/OuterBorderIterator.h"
#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

using namespace std;

void testOuterBorderIterator()
{
	vector<unsigned int> v, w, result;
	OuterBorderIterator<unsigned int>::IteratorState itState;
	DoubleIterator itInterface = outerBorderIterator_create<unsigned int>();

	v=vector<unsigned int>();
	w=vector<unsigned int>();
	itState = outerBorderIteratorState_create(&v);
	runIteratorForward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));
	itState = outerBorderIteratorState_create(&v);
	reverseVector(&w);
	runIteratorBackward(&itState, itInterface, &result);
	test(compareVectors(&result, &w));

	// Skipping case of length 2 because it makes no sense 
	// because it is expected that the last 2 vector components
	// repeat the first 2 and so there are at least 4 vector
	// components

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	w=vector<unsigned int>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	testIterator(v, w, &outerBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	w=vector<unsigned int>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	testIterator(v, w, &outerBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	w=vector<unsigned int>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	w.push_back(5);
	testIterator(v, w, &outerBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	v.push_back(8);
	v.push_back(9);
	w=vector<unsigned int>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	w.push_back(5);
	w.push_back(7);
	testIterator(v, w, &outerBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	v.push_back(8);
	v.push_back(9);
	v.push_back(10);
	v.push_back(11);
	w=vector<unsigned int>();
	// This is correct. Since normally we would expect the last two vector
	// components are the same as the first two, they get skipped
	w.push_back(1);
	w.push_back(3);
	w.push_back(5);
	w.push_back(7);
	w.push_back(9);
	testIterator(v, w, &outerBorderIteratorState_create<unsigned int>, itInterface);
}