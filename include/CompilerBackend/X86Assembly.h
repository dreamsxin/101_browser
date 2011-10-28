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

#ifndef _X86Assembly_h
#define _X86Assembly_h

#include <cstdio>
#include <cstring>
#include <vector>
#include "MiniStdlib/cstdint.h"
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
	DataRegister32EAX = 0,
	DataRegister32ECX = 1,
	DataRegister32EDX = 2,
	DataRegister32EBX = 3,
	SpecialRegister32ESP = 4,
	SpecialRegister32EBP = 5,
	SpecialRegister32ESI = 6,
	SpecialRegister32EDI = 7
};

AssemblyInstruction moveToRegister32(Register32 in_dataRegister, int in_value);
AssemblyInstruction pop_register32(Register32 in_register);
AssemblyInstruction push_imm32(uint32_t in_value);
AssemblyInstruction push_register32(Register32 in_register);
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
