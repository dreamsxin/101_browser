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
