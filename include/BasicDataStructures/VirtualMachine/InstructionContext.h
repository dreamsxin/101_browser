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

#ifndef _InstructionContext_h
#define _InstructionContext_h

// for ReversibleInstructionContext
#include "BasicDataStructures/Util/SingleChain.h"

enum Instruction
{
	Nop, // does nothing
	Jmp, // Changes the Program Counter (PC) in another way than incrementing
	Xchg
};

struct ReversibleMarker;

struct ReversibleContext
{
	// if this is 0 and we run backward, we simple decrement the program counte
	// but if it is not 0, we jump back to the instruction comeFrom 
	// and remove the head
	SingleChain<ReversibleMarker>* markers;

	inline ReversibleContext(Instruction in_instruction) 
		: markers(NULL) { }
};

enum ReversibleMarkerType
{
	Start,
	// No need for a stop marker since when we stop the reversible mode
	// we have to drop all markers
	OK, // does nothing - only a marker that tells to an instruction that 
	    // already has a marker that this time nothing interesting happened
	ComeFrom
};

union ReversibleMarkerData
{
	// Start has no data
	size_t comeFrom;
};

struct ReversibleMarker
{
	ReversibleMarkerType type;
	ReversibleMarkerData data;
};

#endif
