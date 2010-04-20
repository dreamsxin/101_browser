#ifndef _X86Assembly_h
#define _X86Assembly_h

#include <cstdio>
#include <cstring>
#include <vector>
#include <windows.h>

typedef std::vector<unsigned char> AssemblyInstruction;
typedef std::vector<AssemblyInstruction> Program;

enum Register8
{
	al = 0,
	cl = 1,
	dl = 2,
	bl = 3
};

enum Register16
{
	ax = 0,
	cx = 1,
	dx = 2,
	bx = 3
};

enum Register32
{
	eax = 0,
	ecx = 1,
	edx = 2,
	ebx = 3
};

AssemblyInstruction moveToRegister32(Register32 in_register, int in_value);
AssemblyInstruction pop32(Register32 in_register);
AssemblyInstruction push32(int in_value);
AssemblyInstruction ret();

class CompiledProgram
{
protected:
	size_t mProgSize;
	LPVOID mpProg;
	CompiledProgram(const CompiledProgram&) {}

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
