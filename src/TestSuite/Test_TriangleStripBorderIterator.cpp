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

#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "TestSuite/TestSuite.h"
#include "TestSuite/IteratorTestUtil.h"

using namespace std;

void testTriangleStripBorderIterator()
{
	vector<unsigned int> v, w, result;
	DoubleIterator itInterface = triangleStripBorderIterator_create<unsigned int>();

	// Testing correct behaviour for small datasets

	v=vector<unsigned int>();
	w=vector<unsigned int>();
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	w=vector<unsigned int>();
	w.push_back(0);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(2);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	// Now the next two non-trivial cases

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(3);
	w.push_back(2);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);

	v=vector<unsigned int>();
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	w=vector<unsigned int>();
	w.push_back(0);
	w.push_back(1);
	w.push_back(3);
	w.push_back(4);
	w.push_back(2);
	testIterator(v, w, &triangleStripBorderIteratorState_create<unsigned int>, itInterface);
}
