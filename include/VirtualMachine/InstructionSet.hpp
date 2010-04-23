#ifndef _InstructionSet_hpp
#define _InstructionSet_hpp

enum Instruction
{
	// The number tells the number of arguments
	Nop0,
	Halt0, // terminates program execution

	// Prefix F means floating point instruction on double precision
	FNeg1,
	FAdd2,
	FSub2,
	FMul2,
	FDiv2,
	FMod2,

	// Prefix B means it has to do wíth Booleans
	BReset0,
	BSet0,
	BNeg1,
	BAnd2,
	BOr2,
	BXor2,

	FBLess2,
	FBEqual2,
	FBGreater2,
	FBGreaterEqual2,
	FBNotEqual2,
	FBLessEqual2,
	
	// Prefix P means it changes the Program Counter not necessarily 
	// in an incrementing way
	PJmp1,
	BPJmpIfZero1,
	BPJmpIfNotZero1,

	// Prefix S tells that the instruction changes the stack pointer
	SPush1,
	SPop0, // OK, we need to specify where to pop, so 1 instead?...
	SCall1,

	// Prefix I means this is only an information and NOT an instruction
	IInstructionCount
};

#endif
