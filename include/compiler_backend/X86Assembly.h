#ifndef _X86Assembly_h
#define _X86Assembly_h

#include <cstdio>
#include <cstring>
#include <vector>
#include <windows.h>

typedef std::vector<unsigned char> AssemblyInstruction;
typedef std::vector<AssemblyInstruction> Program;

AssemblyInstruction moveToEAX(unsigned int in_value);
AssemblyInstruction moveToEBX(unsigned int in_value);
AssemblyInstruction ret();

class CompiledProgram
{
protected:
	size_t mProgSize;
	LPVOID mpProg;
	CompiledProgram(const CompiledProgram& in_crProgram) {}

public:
	CompiledProgram(Program in_program);
	
	~CompiledProgram();

	inline size_t size()
	{
		return mProgSize;
	}

	inline LPVOID program()
	{
		return mpProg;
	}
};

#endif
