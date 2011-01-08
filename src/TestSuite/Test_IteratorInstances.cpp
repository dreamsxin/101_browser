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

#include "TestSuite/Tests.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "GuiDataStructures/OuterBorderIterator.h"

void testIteratorInstances()
{
	std::vector<unsigned int> vect;
	for (unsigned int i=0; i<6; i++)
		vect.push_back(i);

	DoubleIteratorInstance<TriangleStripBorderIterator<unsigned int>::IteratorState>
		tsbiInstance = triangleStripBorderIteratorInstance_create<unsigned int>(&vect);

	DoubleIteratorInstance<OuterBorderIterator<unsigned int>::IteratorState>
		obiInstance = outerBorderIteratorInstance_create<unsigned int>(&vect);
}
